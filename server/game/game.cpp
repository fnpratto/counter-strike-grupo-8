#include "game.h"

#include <utility>

#include "server/attack_effects/attack_effect.h"
#include "server/errors.h"
#include "server/player_message.h"
#include "server/weapons/bomb.h"

Game::Game(const std::string& name, std::shared_ptr<Clock>&& game_clock, Map&& map):
        Logic<GameState, GameUpdate>(GameState(std::move(game_clock), map.get_max_players())),
        name(name),
        physics_system(std::move(map), state.get_players()) {}

bool Game::is_full() const { return state.game_is_full(); }

GameUpdate Game::tick(const std::vector<PlayerMessage>& msgs) {
    state.clear_updates();
    advance_round_logic();
    for (const PlayerMessage& msg: msgs) handle_msg(msg.get_message(), msg.get_player_name());
    process_attacks();
    advance_players_movement();
    return state.get_updates();
}

std::string Game::get_name() const { return name; }

int Game::get_player_count() const { return static_cast<int>(state.get_players().size()); }

PhaseType Game::get_phase() { return state.get_phase().get_type(); }

void Game::handle_msg(const Message& msg, const std::string& player_name) {
    MessageType msg_type = msg.get_type();
    std::cout << "Game::handle_msg: Received message of type: " << static_cast<int>(msg_type)
              << " from player: " << player_name << "\n";
    if (msg_type == MessageType::SELECT_TEAM_CMD) {
        Team team = msg.get_content<SelectTeamCommand>().get_team();
        handle_select_team_msg(player_name, team);
    } else if (msg_type == MessageType::START_GAME_CMD) {
        handle_start_game_msg(player_name);
    } else if (msg_type == MessageType::BUY_GUN_CMD) {
        GunType gun = msg.get_content<BuyGunCommand>().get_gun();
        handle_buy_gun_msg(player_name, gun);
    } else if (msg_type == MessageType::BUY_AMMO_CMD) {
        ItemSlot slot = msg.get_content<BuyAmmoCommand>().get_slot();
        handle_buy_ammo_msg(player_name, slot);
    } else if (msg_type == MessageType::MOVE_CMD) {
        Vector2D direction = msg.get_content<MoveCommand>().get_direction();
        handle_move_msg(player_name, direction);
    } else if (msg_type == MessageType::STOP_PLAYER_CMD) {
        handle_stop_player_msg(player_name);
    } else if (msg_type == MessageType::AIM_CMD) {
        Vector2D direction = msg.get_content<AimCommand>().get_direction();
        handle_aim_msg(player_name, direction);
    } else if (msg_type == MessageType::ATTACK_CMD) {
        handle_attack_msg(player_name);
    } else if (msg_type == MessageType::SWITCH_ITEM_CMD) {
        ItemSlot slot = msg.get_content<SwitchItemCommand>().get_slot();
        handle_switch_item_msg(player_name, slot);
    } else if (msg_type == MessageType::RELOAD_CMD) {
        handle_reload_msg(player_name);
    }
}

void Game::advance_round_logic() {
    auto& phase = state.get_phase();

    phase.advance();

    if (phase.round_has_finished())
        state.advance_round();

    if (state.get_num_rounds() == GameConfig::max_rounds / 2)
        state.swap_players_teams();
}

void Game::advance_players_movement() {
    std::map<std::string, PlayerUpdate> game_players_update;
    for (const auto& [player_name, player]: state.get_players()) {
        if (player->is_moving()) {
            Vector2D old_pos = player->get_pos();
            Vector2D step = physics_system.calculate_step(player->get_move_dir());
            Vector2D new_pos = old_pos + step;
            // TODO: Check collisions with physics_system (with tiles and entities)
            player->move_to_pos(new_pos);
            game_players_update.emplace(player_name, player->get_updates());
        }
    }
}

void Game::process_attacks() {
    // TODO:
    //      - Access to weapons actually attacking
    //      - perform attack
    //      - if we have an AttackEffect, then:
    //              closest = physics_system.get_closest()
    //              if we have a closest player, then:
    //                      - effect.apply(closest player);
}

GameUpdate Game::join_player(const std::string& player_name) {
    if (player_name.empty())
        throw InvalidPlayerNameError();
    if (state.player_is_in_game(player_name))
        throw JoinGameError();
    if (is_full() || state.get_phase().is_started())
        // TODO: Error response
        return state.get_full_update();

    Team default_team = (state.get_num_tts() > state.get_num_cts()) ? Team::CT : Team::TT;
    if (default_team == Team::TT) {
        Vector2D pos = physics_system.random_spawn_tt_pos();
        state.add_player(player_name, std::make_unique<Player>(default_team, pos));
    } else {
        Vector2D pos = physics_system.random_spawn_ct_pos();
        state.add_player(player_name, std::make_unique<Player>(default_team, pos));
    }

    return state.get_full_update();
}

void Game::handle_select_team_msg(const std::string& player_name, Team team) {
    if (state.get_phase().is_started())
        throw SelectTeamError();
    if (state.team_is_full(team))
        // TODO: Error response
        return;

    state.get_player(player_name)->select_team(team);
}

void Game::handle_start_game_msg(const std::string& player_name) {
    if (state.get_phase().is_started())
        throw StartGameError();

    state.get_player(player_name)->set_ready();
    if (state.all_players_ready()) {
        give_bomb_to_random_tt();
        state.get_phase().start_buying_phase();
    }
}

void Game::handle_buy_gun_msg(const std::string& player_name, GunType gun) {
    if (!state.get_phase().is_buying_phase())
        return;
    if (!physics_system.player_in_spawn(player_name))
        return;

    auto& player = state.get_player(player_name);
    shop.buy_gun(player->get_inventory(), gun);
}

void Game::handle_buy_ammo_msg(const std::string& player_name, ItemSlot slot) {
    if (!state.get_phase().is_buying_phase())
        return;
    if (!physics_system.player_in_spawn(player_name))
        return;

    auto& player = state.get_player(player_name);
    shop.buy_ammo(player->get_inventory(), slot);
}

void Game::handle_move_msg(const std::string& player_name, const Vector2D& direction) {
    if (state.get_phase().is_buying_phase())
        return;

    auto& player = state.get_player(player_name);
    player->start_moving(direction);
}

void Game::handle_stop_player_msg(const std::string& player_name) {
    auto& player = state.get_player(player_name);
    player->stop_moving();
}

void Game::handle_aim_msg(const std::string& player_name, const Vector2D& direction) {
    auto& player = state.get_player(player_name);
    player->aim(direction);
}

void Game::handle_attack_msg(const std::string& player_name) {
    if (!state.get_phase().is_playing_phase())
        return;
    (void)player_name;
    // TODO:
    //      - Current weapon of Player should be set in attacking
    //      - We could keep a reference of the "attacking weapons"
    //        in Game (to access later un process_attacks)
}

void Game::handle_switch_item_msg(const std::string& player_name, ItemSlot slot) {
    auto& player = state.get_player(player_name);
    player->equip_item(slot);
}

void Game::handle_reload_msg(const std::string& player_name) {
    auto& player = state.get_player(player_name);
    player->reload();
}

void Game::give_bomb_to_random_tt() {
    if (state.get_num_tts() == 0)
        return;

    std::vector<std::string> tt_names;

    for (auto& [player_name, player]: state.get_players()) {
        if (player->is_tt())
            tt_names.push_back(player_name);
    }

    int random_index = rand() % state.get_num_tts();

    std::string player_name = tt_names[random_index];
    auto& player = state.get_player(player_name);
    player->pick_bomb(Bomb());
}

Game::~Game() {}

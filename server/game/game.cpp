#include "game.h"

#include <utility>

#include "server/errors.h"
#include "server/player_message.h"

Game::Game(const std::string& name, std::shared_ptr<Clock>&& game_clock, Map&& map):
        Logic<GameState, GameUpdate>(GameState(std::move(game_clock))),
        name(name),
        physics_system(std::move(map), state.get_players()) {}

GameUpdate Game::tick(const std::vector<PlayerMessage>& msgs) {
    state.clear_updates();
    for (const PlayerMessage& msg: msgs) handle_msg(msg.get_message(), msg.get_player_name());
    advance_players_movement();
    advance_round_logic();
    return state.get_updates();
}

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
        GunType gun = msg.get_content<BuyAmmoCommand>().get_gun();
        handle_buy_ammo_msg(player_name, gun);
    } else if (msg_type == MessageType::MOVE_CMD) {
        Vector2D direction = msg.get_content<MoveCommand>().get_direction();
        handle_move_msg(player_name, direction);
    } else if (msg_type == MessageType::STOP_PLAYER_CMD) {
        handle_stop_player_msg(player_name);
    } else if (msg_type == MessageType::AIM_CMD) {
        float x = msg.get_content<AimCommand>().get_x();
        float y = msg.get_content<AimCommand>().get_y();
        handle_aim_msg(player_name, x, y);
        // } else if (msg_type == MessageType::SHOOT_CMD) {
        //     int x = msg.get_content<ShootCommand>().get_x();
        //     int y = msg.get_content<ShootCommand>().get_y();
        //     handle_shoot_msg(player_name, x, y);
    } else if (msg_type == MessageType::SWITCH_WEAPON_CMD) {
        WeaponSlot slot = msg.get_content<SwitchWeaponCommand>().get_slot();
        handle_switch_weapon_msg(player_name, slot);
    } else if (msg_type == MessageType::RELOAD_CMD) {
        handle_reload_msg(player_name);
    }
}

void Game::advance_round_logic() {
    auto& phase = state.get_phase();

    phase.advance();

    if (phase.is_round_finished())
        advance_round();

    if (state.get_num_rounds() == GameConfig::max_rounds / 2)
        swap_teams();
}

void Game::advance_round() { state.set_num_rounds(state.get_num_rounds() + 1); }

void Game::advance_players_movement() {
    std::map<std::string, PlayerUpdate> game_players_update;
    for (const auto& [player_name, player]: state.get_players()) {
        if (player->is_moving()) {
            Vector2D old_pos = player->get_pos();
            Vector2D new_pos = old_pos + physics_system.calculate_step(player->get_move_dir());
            // TODO: Check collisions with physics_system (with tiles and entities)
            player->move_to_pos(new_pos);
            game_players_update.emplace(player_name, player->get_updates());
        }
    }
}

GameUpdate Game::join_player(const std::string& player_name) {
    if (player_name.empty())
        throw InvalidPlayerNameError();
    if (player_is_in_game(player_name) || is_full() || state.get_phase().is_started())
        throw JoinGameError();

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
    const std::unique_ptr<Player>& player = state.get_player(player_name);
    if (state.get_phase().is_started())
        throw SelectTeamError();
    if (team_is_full(team))
        // TODO: Return error response
        return;

    Team old_team = player->is_tt() ? Team::TT : Team::CT;

    if (old_team == team)
        return;

    player->select_team(team);
}

void Game::handle_start_game_msg(const std::string& player_name) {
    if (state.get_phase().is_started())
        throw StartGameError();

    auto& player = state.get_player(player_name);
    player->set_ready();
    if (all_players_ready()) {
        give_bomb_to_random_tt();
        state.get_phase().start_buying_phase();
    }
}

void Game::handle_buy_gun_msg(const std::string& player_name, GunType gun) {
    auto& player = state.get_player(player_name);
    if (!state.get_phase().is_buying_phase())
        throw BuyGunError();
    // if (physics_system.player_not_in_spawn(player))
    //     return;

    shop.buy_gun(player->get_inventory(), gun);
}

// TODO this could take a WeaponSlot instead of GunType
void Game::handle_buy_ammo_msg(const std::string& player_name, GunType gun) {
    auto& player = state.get_player(player_name);
    if (!state.get_phase().is_buying_phase())
        throw BuyAmmoError();
    // if (physics_system.player_not_in_spawn(player))
    //     return;
    WeaponSlot slot = (gun == GunType::Glock) ? WeaponSlot::Secondary : WeaponSlot::Primary;
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

void Game::handle_aim_msg(const std::string& player_name, float x, float y) {
    auto& player = state.get_player(player_name);

    player->aim(x, y);
}

// void Game::handle_shoot_msg(const std::string& player_name, int x, int y) {
//     if (player->get_current_weapon() == WeaponSlot::Melee) {
//         Knife knife = player->attack();
//         Vector2D pos(player->get_pos_x(), player->get_pos_y());
//         physics_system.handle_attack(knife, pos);
//     } else {
//         std::vector<Bullet> bullets = player->shoot_gun(x, y, physics_system.time_now());
//         for (Bullet& b : bullets)
//             physics_system.shoot_bullet(b);
//     }
// }

void Game::handle_switch_weapon_msg(const std::string& player_name, WeaponSlot slot) {
    auto& player = state.get_player(player_name);
    player->equip_weapon(slot);
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
    player->pick_bomb();
}

void Game::swap_teams() {
    for (auto& [_, player]: state.get_players()) {  // cppcheck-suppress[unusedVariable]
        if (player->is_tt()) {
            player->select_team(Team::CT);
        } else if (player->is_ct()) {
            player->select_team(Team::TT);
        }
    }
}

Game::~Game() {}

bool Game::player_is_in_game(const std::string& player_name) const {
    return state.get_players().find(player_name) != state.get_players().end();
}

bool Game::is_full() const {
    return static_cast<int>(state.get_players().size()) == GameConfig::max_players;
}

bool Game::team_is_full(const Team& team) const {
    if (team == Team::TT)
        return state.get_num_tts() == GameConfig::max_team_players;
    return state.get_num_cts() == GameConfig::max_team_players;
}

bool Game::all_players_ready() const {
    for (auto& [_, player]: state.get_players()) {  // cppcheck-suppress[unusedVariable]
        if (!player->is_ready())
            return false;
    }
    return true;
}

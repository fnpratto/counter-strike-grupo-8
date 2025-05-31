#include "game.h"

#include <utility>

#include "server/errors.h"

Game::Game(const std::string& name, std::unique_ptr<Clock>&& game_clock, Map&& map):
        name(name),
        state(std::move(game_clock)),
        physics_system(std::move(map), state.get_players()) {
    std::srand(std::time(nullptr));
}

GameUpdate Game::tick(const std::vector<Message>& msgs, const std::string& player_name) {
    state.clear_updates();
    for (const Message& msg: msgs) {
        handle_msg(msg, player_name);
    }
    advance_players_movement();
    advance_round_logic();
    return state.get_updates();
}

void Game::handle_msg(const Message& msg, const std::string& player_name) {
    MessageType msg_type = msg.get_type();
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
        int dx = msg.get_content<MoveCommand>().get_dx();
        int dy = msg.get_content<MoveCommand>().get_dy();
        handle_move_msg(player_name, dx, dy);
    } else if (msg_type == MessageType::STOP_CMD) {
        handle_stop_msg(player_name);
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
        state.advance_round();

    if (state.get_num_rounds() == GameConfig::max_rounds / 2)
        swap_teams();
}

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

const GameState& Game::join_player(const std::string& player_name) {
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

    return state;
}

void Game::handle_select_team_msg(const std::string& player_name, Team team) {
    const std::unique_ptr<Player>& player = state.get_players().at(player_name);
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

    auto& player = state.get_players().at(player_name);
    player->set_ready();
    if (all_players_ready()) {
        give_bomb_to_random_tt();
        state.get_phase().start_buying_phase();
    }
}

void Game::handle_buy_gun_msg(const std::string& player_name, GunType gun) {
    auto& player = state.get_players().at(player_name);
    if (!state.get_phase().is_buying_phase())
        throw BuyGunError();
    // if (physics_system.player_not_in_spawn(player))
    //     return;
    int gun_price = shop.get_gun_price(gun);
    player->buy_gun(gun, gun_price);
}

void Game::handle_buy_ammo_msg(const std::string& player_name, GunType gun) {
    auto& player = state.get_players().at(player_name);
    if (!state.get_phase().is_buying_phase())
        throw BuyAmmoError();
    // if (physics_system.player_not_in_spawn(player))
    //     return;
    int num_mags = 1;
    if (gun == GunType::M3)
        num_mags = 8;
    WeaponSlot slot = (gun == GunType::Glock) ? WeaponSlot::Secondary : WeaponSlot::Primary;
    int ammo_price = shop.get_ammo_price(gun, num_mags);
    player->buy_ammo(slot, ammo_price, num_mags);
}

void Game::handle_move_msg(const std::string& player_name, int dx, int dy) {
    if (state.get_phase().is_buying_phase())
        return;

    auto& player = state.get_players().at(player_name);

    player->start_moving(dx, dy);
}

void Game::handle_stop_msg(const std::string& player_name) {
    auto& player = state.get_players().at(player_name);

    player->stop_moving();
}

void Game::handle_aim_msg(const std::string& player_name, float x, float y) {
    auto& player = state.get_players().at(player_name);

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
    auto& player = state.get_players().at(player_name);
    player->equip_weapon(slot);
}

void Game::handle_reload_msg(const std::string& player_name) {
    auto& player = state.get_players().at(player_name);
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
    auto& player = state.get_players().at(player_name);
    player->pick_bomb();
}

void Game::swap_teams() {
    for (auto& [player_name, player]: state.get_players()) {
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

const GameState& Game::get_state() const { return state; }

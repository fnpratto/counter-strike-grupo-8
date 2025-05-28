#include "game.h"

#include <utility>

#include "server/errors.h"

Game::Game(const std::string& name, const Clock& clock, const Map& map):
        name(name), phase(clock), physics_system(map, players) {
    std::srand(std::time(nullptr));
}

GameUpdate Game::tick(const std::vector<Message>& msgs, const std::string& player_name) {
    clear_updates();
    for (const Message& msg: msgs) {
        handle_msg(msg, player_name);
    }
    advance_players_movement();
    advance_round_logic();
    return updates;
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
    phase.advance();
    updates.add_change(GameStateAttr::PHASE, phase.get_updates());
    if (phase.is_round_finished()) {
        num_rounds++;
        updates.add_change(GameStateAttr::NUM_ROUNDS, num_rounds);
    }
    if (num_rounds == GameConfig::max_rounds / 2)
        swap_teams();
}

void Game::advance_players_movement() {
    for (const auto& [_, p]: players) {
        if (p->is_moving()) {
            Vector2D old_pos = p->get_pos();
            Vector2D new_pos = old_pos + physics_system.calculate_step(p->get_move_dir());
            // TODO: Check collisions with physics_system (with tiles and entities)
            p->move_to_pos(new_pos);
        }
    }
}

GameState Game::join_player(const std::string& player_name) {
    if (player_name.empty())
        throw InvalidPlayerNameError();
    if (player_is_in_game(player_name) || is_full() || phase.is_started())
        throw JoinGameError();

    Team team = (num_tts > num_cts) ? Team::CT : Team::TT;
    if (team == Team::TT) {
        Vector2D pos = physics_system.random_spawn_tt_pos();
        players[player_name] = std::make_unique<Player>(team, pos);
        num_tts++;
    } else {
        Vector2D pos = physics_system.random_spawn_ct_pos();
        players[player_name] = std::make_unique<Player>(team, pos);
        num_cts++;
    }

    return full_state();
}

void Game::handle_select_team_msg(const std::string& player_name, Team team) {
    std::unique_ptr<Player>& player = players.at(player_name);
    if (phase.is_started())
        throw SelectTeamError();
    if (team_is_full(team))
        // TODO: Return error response
        return;

    Team old_team = player->is_tt() ? Team::TT : Team::CT;

    if (old_team == team)
        return;

    player->select_team(team);

    if (team == Team::TT) {
        num_cts--;
        num_tts++;
    } else {
        num_tts--;
        num_cts++;
    }
    updates.add_change(GameStateAttr::NUM_TTS, num_tts);

    std::map<std::string, PlayerUpdate> game_players_update;
    game_players_update.emplace(player_name, player->get_updates());
    updates.add_change(GameStateAttr::PLAYERS, game_players_update);
}

void Game::handle_start_game_msg(const std::string& player_name) {
    if (phase.is_started())
        throw StartGameError();

    std::unique_ptr<Player>& player = players.at(player_name);
    player->set_ready();
    if (all_players_ready()) {
        give_bomb_to_random_tt();
        phase.start_buying_phase();
        updates.add_change(GameStateAttr::PHASE, phase.get_updates());
    }

    std::map<std::string, PlayerUpdate> game_players_update;
    game_players_update.emplace(player_name, player->get_updates());
    updates.add_change(GameStateAttr::PLAYERS, game_players_update);
}

void Game::handle_buy_gun_msg(const std::string& player_name, GunType gun) {
    std::unique_ptr<Player>& player = players.at(player_name);
    if (!phase.is_buying_phase())
        throw BuyGunError();
    // if (physics_system.player_not_in_spawn(player))
    //     return;
    int gun_price = shop.get_gun_price(gun);
    player->buy_gun(gun, gun_price);

    std::map<std::string, PlayerUpdate> game_players_update;
    game_players_update.emplace(player_name, player->get_updates());
    updates.add_change(GameStateAttr::PLAYERS, game_players_update);
}

void Game::handle_buy_ammo_msg(const std::string& player_name, GunType gun) {
    std::unique_ptr<Player>& player = players.at(player_name);
    if (!phase.is_buying_phase())
        throw BuyAmmoError();
    // if (physics_system.player_not_in_spawn(player))
    //     return;
    int num_mags = 1;
    if (gun == GunType::M3)
        num_mags = 8;
    WeaponSlot slot = (gun == GunType::Glock) ? WeaponSlot::Secondary : WeaponSlot::Primary;
    int ammo_price = shop.get_ammo_price(gun, num_mags);
    player->buy_ammo(slot, ammo_price, num_mags);

    std::map<std::string, PlayerUpdate> game_players_update;
    game_players_update.emplace(player_name, player->get_updates());
    updates.add_change(GameStateAttr::PLAYERS, game_players_update);
}

void Game::handle_move_msg(const std::string& player_name, int dx, int dy) {
    if (phase.is_buying_phase())
        return;

    std::unique_ptr<Player>& player = players.at(player_name);

    player->start_moving(dx, dy);

    std::map<std::string, PlayerUpdate> game_players_update;
    game_players_update.emplace(player_name, player->get_updates());
    updates.add_change(GameStateAttr::PLAYERS, game_players_update);
}

void Game::handle_stop_msg(const std::string& player_name) {
    std::unique_ptr<Player>& player = players.at(player_name);

    player->stop_moving();

    std::map<std::string, PlayerUpdate> game_players_update;
    game_players_update.emplace(player_name, player->get_updates());
    updates.add_change(GameStateAttr::PLAYERS, game_players_update);
}

void Game::handle_aim_msg(const std::string& player_name, float x, float y) {
    std::unique_ptr<Player>& player = players.at(player_name);

    player->aim(x, y);

    std::map<std::string, PlayerUpdate> game_players_update;
    game_players_update.emplace(player_name, player->get_updates());
    updates.add_change(GameStateAttr::PLAYERS, game_players_update);
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
    std::unique_ptr<Player>& player = players.at(player_name);
    player->equip_weapon(slot);

    std::map<std::string, PlayerUpdate> game_players_update;
    game_players_update.emplace(player_name, player->get_updates());
    updates.add_change(GameStateAttr::PLAYERS, game_players_update);
}

void Game::handle_reload_msg(const std::string& player_name) {
    std::unique_ptr<Player>& player = players.at(player_name);
    player->reload();

    std::map<std::string, PlayerUpdate> game_players_update;
    game_players_update.emplace(player_name, player->get_updates());
    updates.add_change(GameStateAttr::PLAYERS, game_players_update);
}

void Game::give_bomb_to_random_tt() {
    if (num_tts == 0)
        return;

    std::vector<std::string> tt_names;

    for (auto& [player_name, player]: players) {
        if (player->is_tt())
            tt_names.push_back(player_name);
    }

    int random_index = rand() % num_tts;

    std::string player_name = tt_names[random_index];
    std::unique_ptr<Player>& player = players.at(player_name);
    player->pick_bomb();

    std::map<std::string, PlayerUpdate> game_players_update;
    game_players_update.emplace(player_name, player->get_updates());
    updates.add_change(GameStateAttr::PLAYERS, game_players_update);
}

void Game::swap_teams() {
    std::map<std::string, PlayerUpdate> game_players_update;

    for (auto& [player_name, player]: players) {
        if (player->is_tt()) {
            player->select_team(Team::CT);
        } else if (player->is_ct()) {
            player->select_team(Team::TT);
        }
        game_players_update.emplace(player_name, player->get_updates());
    }

    updates.add_change(GameStateAttr::PLAYERS, game_players_update);
}

Game::~Game() {}

void Game::clear_updates() {
    updates.clear();
    phase.clear_updates();
    for (auto& [_, p]: players) {
        p->clear_updates();
    }
}

bool Game::player_is_in_game(const std::string& player_name) const {
    return players.find(player_name) != players.end();
}

bool Game::is_full() const { return static_cast<int>(players.size()) == GameConfig::max_players; }

bool Game::team_is_full(const Team& team) const {
    if (team == Team::TT)
        return num_tts == GameConfig::max_team_players;
    return num_cts == GameConfig::max_team_players;
}

bool Game::all_players_ready() const {
    for (auto& [_, player]: players) {
        if (!player->is_ready())
            return false;
    }
    return true;
}

GameState Game::full_state() {
    GameState game_state;

    std::map<std::string, PlayerState> players_states;
    for (auto& [player_name, player]: players) {
        players_states[player_name] = player->full_state();
    }

    game_state.phase = phase.full_state();
    game_state.num_rounds = num_rounds;
    game_state.num_tts = num_tts;
    game_state.num_cts = num_cts;
    game_state.players = players_states;

    return game_state;
}

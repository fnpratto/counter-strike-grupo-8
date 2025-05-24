#include "game.h"

#include <utility>
#include <vector>

#include "server/cons.h"
#include "server/errors.h"

using GameConfig::tickrate;
using PlayerInitialConfig::full_health;

Game::Game(const std::string& name, const Clock& clock, const Map& map):
        name(name), phase(clock), physics_system(map, players) {
    std::srand(std::time(nullptr));
}

void Game::tick(Message msg, const std::string& player_name) {
    handle_msg(msg, player_name);
    update();
}

void Game::handle_msg(Message msg, const std::string& player_name) {
    std::unique_ptr<Player>& player = players.at(player_name);
    MessageType msg_type = msg.get_type();
    if (msg_type == MessageType::SELECT_TEAM_CMD) {
        Team team = msg.get_content<SelectTeamCommand>().get_team();
        handle_select_team_msg(player, team);
    } else if (msg_type == MessageType::START_GAME_CMD) {
        handle_start_game_msg(player);
    } else if (msg_type == MessageType::BUY_GUN_CMD) {
        GunType gun = msg.get_content<BuyGunCommand>().get_gun();
        handle_buy_gun_msg(player, gun);
    } else if (msg_type == MessageType::BUY_AMMO_CMD) {
        GunType gun = msg.get_content<BuyAmmoCommand>().get_gun();
        handle_buy_ammo_msg(player, gun);
    } else if (msg_type == MessageType::MOVE_CMD) {
        int dx = msg.get_content<MoveCommand>().get_dx();
        int dy = msg.get_content<MoveCommand>().get_dy();
        handle_move_msg(player, dx, dy);
        // } else if (msg_type == MessageType::SHOOT_CMD) {
        //     int x = msg.get_content<ShootCommand>().get_x();
        //     int y = msg.get_content<ShootCommand>().get_y();
        //     handle_shoot_msg(player, x, y);
    } else if (msg_type == MessageType::SWITCH_WEAPON_CMD) {
        WeaponSlot slot = msg.get_content<SwitchWeaponCommand>().get_slot();
        player->equip_weapon(slot);
    } else if (msg_type == MessageType::RELOAD_CMD) {
        player->reload();
    }
}

void Game::update() {
    phase.update();
    if (phase.is_round_finished())
        num_rounds++;
    if (num_rounds == max_rounds / 2)
        swap_teams();
}

GameUpdate Game::join_player(const std::string& player_name) {
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

    return state_update();
}

void Game::handle_select_team_msg(std::unique_ptr<Player>& player, Team team) {
    if (phase.is_started())
        throw SelectTeamError();
    if (team_is_full(team))
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
}

void Game::handle_start_game_msg(std::unique_ptr<Player>& player) {
    player->set_ready();
    if (all_players_ready()) {
        give_bomb_to_random_tt();
        phase.start_buying_phase();
    }
}

void Game::handle_buy_gun_msg(std::unique_ptr<Player>& player, GunType gun) {
    if (!phase.is_buying_phase())
        throw BuyGunError();
    // if (physics_system.player_not_in_spawn(player))
    //     return;
    int gun_price = shop.get_gun_price(gun);
    player->buy_gun(gun, gun_price);
}

void Game::handle_buy_ammo_msg(std::unique_ptr<Player>& player, GunType gun) {
    if (!phase.is_buying_phase())
        throw BuyAmmoError();
    // if (physics_system.player_not_in_spawn(player))
    //     return;
    int num_mags = 1;
    if (gun == GunType::M3) {
        num_mags = 8;
    }
    WeaponSlot slot = (gun == GunType::Glock) ? WeaponSlot::Secondary : WeaponSlot::Primary;
    int ammo_price = shop.get_ammo_price(gun, num_mags);
    player->buy_ammo(slot, ammo_price, num_mags);
}

void Game::handle_move_msg(std::unique_ptr<Player>& player, int dx, int dy) {
    if (phase.is_buying_phase())
        return;

    Vector2D old_pos = player->get_pos();
    Vector2D new_pos = old_pos + physics_system.calculate_step(Vector2D(dx, dy));

    // TODO: Check collision with physics_system

    player->move(new_pos);
}

// void Game::handle_shoot_msg(std::unique_ptr<Player>& player, int x, int y) {
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

void Game::give_bomb_to_random_tt() {
    if (num_tts == 0)
        return;

    std::vector<std::string> tt_names;

    for (auto& [player_name, player]: players) {
        if (player->is_tt())
            tt_names.push_back(player_name);
    }

    int random_index = rand() % num_tts;
    players.at(tt_names[random_index])->pick_bomb();
}

void Game::swap_teams() {
    for (auto& [_, player]: players) {
        if (player->is_tt()) {
            player->select_team(Team::CT);
        } else if (player->is_ct()) {
            player->select_team(Team::TT);
        }
    }
}

Game::~Game() {}

bool Game::player_is_in_game(const std::string& player_name) const {
    return players.find(player_name) == players.end();
}

bool Game::is_full() const { return static_cast<int>(players.size()) == max_players; }

bool Game::team_is_full(const Team& team) const {
    if (team == Team::TT)
        return num_tts == max_team_players;
    return num_cts == max_team_players;
}

bool Game::all_players_ready() const {
    for (auto& [_, player]: players) {
        if (!player->is_ready())
            return false;
    }
    return true;
}

GameUpdate Game::state_update() {
    GameUpdate gs_upd;

    std::map<std::string, PlayerState> players_states;
    for (auto& [player_name, player]: players) {
        players_states[player_name] = player->state();
    }
    gs_upd.add_change(GameStateAttr::PLAYERS, players_states);

    gs_upd.add_change(GameStateAttr::NUM_ROUNDS, num_rounds);
    gs_upd.add_change(GameStateAttr::NUM_TTS, num_tts);
    gs_upd.add_change(GameStateAttr::NUM_PLAYERS, static_cast<int>(players.size()));
    gs_upd.add_change(GameStateAttr::PLAYERS, std::map<std::string, PlayerState>{});

    return gs_upd;
}

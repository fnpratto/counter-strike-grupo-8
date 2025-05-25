#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "common/game_state_update.h"
#include "common/message.h"
#include "server/clock/clock.h"
#include "server/map/map.h"
#include "server/player/player.h"

#include "game_config.h"
#include "game_phase.h"
#include "physics_system.h"
#include "shop.h"

class Game {
private:
    std::string name;
    Shop shop;
    GamePhase phase;
    std::map<std::string, std::unique_ptr<Player>> players;
    PhysicsSystem physics_system;
    GameUpdate updates;
    int num_rounds = 0;
    int num_tts = 0;
    int num_cts = 0;

    bool player_is_in_game(const std::string& player_name) const;
    bool team_is_full(const Team& team) const;
    bool all_players_ready() const;

    void give_bomb_to_random_tt();

    void swap_teams();

    void handle_msg(const Message& msg, const std::string& player_name);
    void handle_select_team_msg(const std::string& player_name, Team team);
    void handle_start_game_msg(const std::string& player_name);
    void handle_buy_gun_msg(const std::string& player_name, GunType gun);
    void handle_buy_ammo_msg(const std::string& player_name, GunType gun);
    void handle_move_msg(const std::string& player_name, int dx, int dy);
    void handle_aim_msg(const std::string& player_name, float dx, float dy);

    // TODO: implement shoot command
    // void handle_shoot_msg(const std::string& player_name, int x, int y);

    void handle_switch_weapon_msg(const std::string& player_name, WeaponSlot slot);
    void handle_reload_msg(const std::string& player_name);

    GameState full_state();

    void advance_round_logic();

public:
    Game(const std::string& name, const Clock& clock, const Map& map);

    bool is_full() const;

    GameState join_player(const std::string& player_name);

    GameUpdate tick(const std::vector<Message>& msgs, const std::string& player_name);

    ~Game();
};

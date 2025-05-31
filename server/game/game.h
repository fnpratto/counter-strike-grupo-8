#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "common/game_state.h"
#include "common/message.h"
#include "common/updates/game_update.h"
#include "server/clock/clock.h"
#include "server/logic.h"
#include "server/map/map.h"
#include "server/player/player.h"

#include "game_config.h"
#include "game_phase.h"
#include "physics_system.h"
#include "shop.h"

class Game: public Logic<GameState, GameUpdate> {
private:
    std::string name;

    Shop shop;
    PhysicsSystem physics_system;

public:
    Game(const std::string& name, std::unique_ptr<Clock>&& game_clock, Map&& map);

    bool is_full() const;

    GameUpdate join_player(const std::string& player_name);

    GameUpdate tick(const std::vector<Message>& msgs, const std::string& player_name);

    ~Game();

private:
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
    void handle_move_msg(const std::string& player_name, const Vector2D& direction);
    void handle_stop_msg(const std::string& player_name);
    void handle_aim_msg(const std::string& player_name, float x, float y);

    // TODO: implement shoot command
    // void handle_shoot_msg(const std::string& player_name, int x, int y);

    void handle_switch_weapon_msg(const std::string& player_name, WeaponSlot slot);
    void handle_reload_msg(const std::string& player_name);


    void advance_players_movement();
    void advance_round_logic();
};

#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "common/message.h"
#include "common/updates/game_update.h"
#include "server/clock/clock.h"
#include "server/logic.h"
#include "server/map/map.h"
#include "server/player/player.h"
#include "server/player_message.h"
#include "server/states/game_state.h"

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
    Game(const std::string& name, std::shared_ptr<Clock>&& game_clock, Map&& map);

    bool is_full() const;

    GameUpdate join_player(const std::string& player_name);

    GameUpdate tick(const std::vector<PlayerMessage>& msgs);

    ~Game();

private:
    bool team_is_full(const Team& team) const;

    void give_bomb_to_random_tt();

    void handle_msg(const Message& msg, const std::string& player_name);
    void handle_select_team_msg(const std::string& player_name, Team team);
    void handle_start_game_msg(const std::string& player_name);
    void handle_buy_gun_msg(const std::string& player_name, GunType gun);
    void handle_buy_ammo_msg(const std::string& player_name, ItemSlot slot);
    void handle_move_msg(const std::string& player_name, const Vector2D& direction);
    void handle_stop_player_msg(const std::string& player_name);
    void handle_aim_msg(const std::string& player_name, const Vector2D& direction);
    void handle_attack_msg(const std::string& player_name);
    void handle_switch_item_msg(const std::string& player_name, ItemSlot slot);
    void handle_reload_msg(const std::string& player_name);

    void advance_round_logic();
    void advance_players_movement();
    void process_attacks();
};

#pragma once

#include <map>
#include <memory>
#include <string>

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
    int num_rounds = 0;
    int num_tts = 0;
    int num_cts = 0;

    bool player_is_in_game(const std::string& player_name) const;
    bool team_is_full(const Team& team) const;
    bool all_players_ready() const;

    void give_bomb_to_random_tt();

    void swap_teams();

    void handle_msg(Message msg, const std::string& player_name);
    void handle_select_team_msg(std::unique_ptr<Player>& player, Team team);
    void handle_start_game_msg(std::unique_ptr<Player>& player);
    void handle_buy_gun_msg(std::unique_ptr<Player>& player, GunType gun);
    void handle_buy_ammo_msg(std::unique_ptr<Player>& player, GunType gun);
    void handle_move_msg(std::unique_ptr<Player>& player, int dx, int dy);

    // TODO: implement shoot command
    // void handle_shoot_msg(std::unique_ptr<Player>& player, int x, int y);

    GameUpdate state_update();

public:
    Game(const std::string& name, const Clock& clock, const Map& map);

    bool is_full() const;

    GameUpdate join_player(const std::string& player_name);

    void tick(Message msg, const std::string& player_name);

    void update();

    ~Game();
};

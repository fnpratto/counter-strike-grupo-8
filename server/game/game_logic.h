#pragma once

#include <string>
#include <map>

#include "game_phase.h"
#include "shop.h"
#include "game_state.h"
#include "server/cons.h"
#include "server/player/player.h"
#include "server/clock/clock.h"

class GameLogic {
private:
    GamePhase phase;
    const Shop& shop;
    std::map<std::string, Player> players;
    int num_rounds = 0;
    int num_tts = 0;
    int num_cts = 0;
    friend class GameStateBuilder;

    Team choose_player_team();
    void give_bomb_to_random_tt();
    
    void swap_teams();

    bool can_join(const std::string& player_name) const;
    bool player_can_select_team(Team& team) const;

    bool is_full() const;
    bool is_started() const;
    bool team_is_full(Team& team) const;

public:
    GameLogic(const Clock& clock, const Shop& shop);

    void add_player(const std::string& player_name);
    Vector2D set_initial_pos();
    void select_team(const std::string& player_name, Team team);    
    void start_game();
    void buy_gun(const std::string& player_name, GunType gun);
    void buy_ammo(const std::string& player_name, GunType gun);
    void move(const std::string& player_name, int dx, int dy);

    void update_round_phase();

    GameState get_game_state() const;

    ~GameLogic();
};

#pragma once

#include <string>
#include <map>

#include "game_config.h"
#include "game_phase.h"
#include "server/player/player.h"
#include "server/clock/clock.h"

class GameState {
private:
    GameConfig conf;
    GamePhase phase;

    Team choose_player_team();
    void give_bomb_to_random_tt();
    
    void swap_teams();

    bool can_join(const std::string& player_name) const;
    bool player_can_select_team(Team& team) const;

    bool is_full() const;
    bool is_started() const;
    bool team_is_full(Team& team) const;
public:
    GameState(const GameConfig& conf, const Clock& clock);

    GameConfig get_config();
    PhaseType get_phase();

    void add_player(const std::string& player_name);
    Vector2D set_init_pos();
    void select_team(const std::string& player_name, Team team);    
    void start_game();
    void buy_gun(const std::string& player_name, GunType gun, int gun_price);
    void buy_ammo(const std::string& player_name, GunType gun, int mag_price);
    void move(const std::string& player_name, int dx, int dy);

    void update_round_phase();

    ~GameState();
};

#pragma once

#include <map>

#include "common/models.h"
#include "server/player/player.h"

class GameConfig {
private:
    friend class GameState;

    float tickrate = 60; // ticks per second
    int num_rounds = 0;
    int max_rounds = 10;
    int num_tts = 0;
    int num_cts = 0;
    
    int max_players = 10;
    int max_team_players = max_players / 2;
    int full_health = 100;
    float player_speed = 180; // pixels per second
    
    int buying_phase_secs = 30;
    int playing_phase_secs = 120;
    int round_finished_phase_secs = 5;
    
    std::map<std::string, Player> players;
    Inventory init_invent;

public:
    GameConfig() {}

    // TODO: add constructor to set a specific configuration.

    float get_tickrate() { return tickrate; };
    int get_num_rounds() { return num_rounds; }
    int get_max_rounds() { return max_rounds; }
    int get_num_tts() { return num_tts; }
    int get_num_cts() { return num_cts; }

    int get_max_players() { return max_players; }
    int get_max_team_players() { return max_team_players; }
    int get_full_health() { return full_health; }
    float get_player_speed() { return player_speed; }
    
    int get_buying_phase_secs() { return buying_phase_secs; }
    int get_playing_phase_secs() { return playing_phase_secs; }
    int get_round_finished_phase_secs() { return round_finished_phase_secs; }
    Inventory get_init_invent() { return init_invent; }

    int get_num_players() { return players.size(); }
    Player get_player(const std::string& player_name) { 
        return players.at(player_name);
    }
    
    bool is_full() { return max_players == num_tts + num_cts; }

    void set_init_invent(Inventory inventory) { init_invent = inventory; }
};

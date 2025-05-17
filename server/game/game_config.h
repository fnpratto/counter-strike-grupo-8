#pragma once

#include <map>

#include "common/models.h"
#include "server/player/player.h"

class GameConfig {
private:
    friend class GameState;

    int num_rounds = 0;
    int num_tts = 0;
    int num_cts = 0;
    
    int max_rounds = 10;
    int max_players = 10;
    int max_team_players = max_players / 2;
    int full_health = 100;
    int buying_phase_secs = 30;
    int playing_phase_secs = 120;
    int round_finished_phase_secs = 5;
    
    std::map<std::string, Player> players;
    Inventory default_inventory;

public:
    GameConfig() {}

    // TODO: add constructor to set a specific configuration.

    int get_num_rounds() { return num_rounds; }
    int get_num_tts() { return num_tts; }
    int get_num_cts() { return num_cts; }
    int get_max_rounds() { return max_rounds; }
    int get_max_players() { return max_players; }
    int get_max_team_players() { return max_team_players; }
    int get_full_health() { return full_health; }
    int get_buying_phase_secs() { return buying_phase_secs; }
    int get_playing_phase_secs() { return playing_phase_secs; }
    int get_round_finished_phase_secs() { return round_finished_phase_secs; }
    Inventory get_default_inventory() { return default_inventory; }

    int get_num_players() { return players.size(); }
    Player get_player(const std::string& player_name) { 
        return players.at(player_name);
    }
    
    bool is_full() { return max_players == num_tts + num_cts; }

    void set_default_inventory(Inventory inventory) { default_inventory = inventory; }
};

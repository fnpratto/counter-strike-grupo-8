#pragma once

#include <map>

#include "common/models.h"
#include "inventory.h"

class GameConfig {
private:
    friend class GameState;

    unsigned int num_rounds = 0;
    unsigned int num_tts = 0;
    unsigned int num_cts = 0;
    
    unsigned int max_rounds = 10;
    unsigned int max_players = 10;
    unsigned int max_players_per_team = max_players / 2;
    unsigned int full_health = 100;
    unsigned int buying_phase_secs = 30;
    unsigned int playing_phase_secs = 120;
    unsigned int round_finished_phase_secs = 5;
    
    std::map<std::string, Player> players;
    Inventory default_inventory;
public:
    GameConfig() {
        default_inventory.money = 800;
        default_inventory.weapons = {
            {WeaponSlot::Secondary, WeaponType::Glock},
            {WeaponSlot::Melee, WeaponType::Knife}
        };
    }

    // TODO: add constructor to set a specific configuration.

    unsigned int get_num_rounds() { return num_rounds; }
    unsigned int get_num_tts() { return num_tts; }
    unsigned int get_num_cts() { return num_cts; }
    unsigned int get_max_rounds() { return max_rounds; }
    unsigned int get_max_players() { return max_players; }
    unsigned int get_max_players_per_team() { return max_players_per_team; }
    unsigned int get_full_health() { return full_health; }
    unsigned int get_buying_phase_secs() { return buying_phase_secs; }
    unsigned int get_playing_phase_secs() { return playing_phase_secs; }
    unsigned int get_round_finished_phase_secs() { return round_finished_phase_secs; }

    int get_num_players() { return players.size(); }
    Inventory get_default_inventory() { return default_inventory; }
    
    Player get_player(const std::string& player_name) { 
        return players.at(player_name);
    }
    
    bool is_full() { return max_players == num_tts + num_cts; }

    void set_default_inventory(Inventory inventory) { default_inventory = inventory; }
};

#pragma once

#include <map>

#include "common/models.h"
#include "inventory.h"

class GameConfig {
private:
    unsigned int max_players;
    Inventory default_inventory;
    int buying_phase_secs;
    int playing_phase_secs;

public:
    GameConfig() :
            max_players(10),
            buying_phase_secs(30),
            playing_phase_secs(120) {
        default_inventory.money = 800;
        default_inventory.weapons = {
            {WeaponSlot::Secondary, WeaponType::Glock},
            {WeaponSlot::Melee, WeaponType::Knife}
        };
    }

    unsigned int get_max_players_game() const { return max_players; }
    unsigned int get_max_players_team() const { return max_players / 2; }
    int get_buying_phase_secs() const { return buying_phase_secs; }
    int get_playing_phase_secs() const { return playing_phase_secs; }
    Inventory get_default_inventory() const { return default_inventory; }
    
    void set_default_inventory(Inventory config_inventory) { 
        default_inventory = config_inventory;
    }
};

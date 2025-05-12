#pragma once

#include <map>

#include "common/models.h"
#include "inventory.h"

class GameConfig {
private:
    unsigned int max_players;
    Inventory initial_inventory;
    int buying_phase_secs;
    int playing_phase_secs;

public:
    GameConfig() :
            max_players(10),
            buying_phase_secs(30),
            playing_phase_secs(120) {
        initial_inventory.money = 800;
        initial_inventory.weapons = {
            {WeaponSlot::Secondary, WeaponType::Glock},
            {WeaponSlot::Melee, WeaponType::Knife},
            {WeaponSlot::Bomb, WeaponType::C4}
        };
    }

    unsigned int get_max_players() const { return max_players; }
    int get_buying_phase_secs() const { return buying_phase_secs; }
    int get_playing_phase_secs() const { return playing_phase_secs; }
    Inventory get_initial_inventory() const { return initial_inventory; }
    
    void set_initial_inventory(Inventory config_inventory) { 
        initial_inventory = config_inventory;
    }
};

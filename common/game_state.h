#pragma once

#include <map>
#include <string>

#include "models.h"

struct GunState {
    GunType gun;
    int bullets_per_mag;
    int mag_ammo;
    int reserve_ammo;
};

struct UtilityState {
    UtilityType utility;
};

struct InventoryState {
    std::map<WeaponSlot, GunState> guns;
    std::map<WeaponSlot, UtilityState> utilities;
};

struct PlayerState {
    Team team;
    float pos_x;
    float pos_y;
    float aim_x;
    float aim_y;
    bool ready;
    int health;
    int money;
    WeaponSlot current_weapon;
    InventoryState inventory;
};

struct GameState {
    PhaseType phase;
    int num_rounds;
    int num_tts;
    int num_cts;
    std::map<std::string, PlayerState> players;
};

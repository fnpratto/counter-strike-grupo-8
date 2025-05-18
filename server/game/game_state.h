#pragma once

#include <map>

#include "common/models.h"

struct GunState {
    GunType gun;
    int bullets_per_mag;
    int mag_ammo;
    int reserve_ammo;
};

struct InventoryState {
    int money;
    std::map<WeaponSlot, GunState> guns;
    std::map<WeaponSlot, UtilityType> utilities;
};

struct PlayerState {
    Team team;
    InventoryState inventory;
    int health;
    float pos_x;
    float pos_y;
    WeaponSlot current_weapon;
};

struct GameState {
    PhaseType phase;
    int num_rounds;
    int num_tts;
    int num_cts;
    std::map<std::string, PlayerState> players;
};

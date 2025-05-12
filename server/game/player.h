#pragma once

#include "common/models.h"
#include "inventory.h"

class Player {
private:
    Team team;
    Inventory inventory;

public:
    Player(Team team, Inventory inventory);

    Team get_team() const;
    Inventory get_inventory() const;

    void gain_money(int amount);

    void buy_weapon(const WeaponType& weapon, int weapon_price);

    ~Player();
};

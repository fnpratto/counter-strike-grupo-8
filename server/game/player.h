#pragma once

#include "common/models.h"
#include "inventory.h"

class Player {
private:
    Team team;
    Inventory inventory;

public:
    Player(Team team, Inventory inventory);

    Inventory get_inventory() const;
    bool is_terrorist() const;
    bool is_counter_terrorist() const;

    void gain_money(int amount);
    void pick_bomb(WeaponType bomb);

    void select_team(Team team);
    void buy_weapon(const WeaponType& weapon, int weapon_price);

    ~Player();
};

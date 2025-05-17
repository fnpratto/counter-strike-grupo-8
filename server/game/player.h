#pragma once

#include "common/models.h"
#include "inventory.h"

class Player {
private:
    Team team;
    Inventory inventory;
    int health;

public:
    Player(Team team, Inventory inventory, int health);

    Inventory get_inventory() const;
    int get_health() const;
    bool is_tt() const;
    bool is_ct() const;

    void gain_money(int amount);
    void pick_bomb(WeaponType bomb);

    void select_team(Team team);
    void buy_weapon(const WeaponType& weapon, int weapon_price);

    ~Player();
};

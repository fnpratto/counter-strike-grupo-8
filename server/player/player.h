#pragma once

#include "common/models.h"
#include "inventory.h"
#include "server/utils/vector_2d.h"

class Player {
private:
    Team team;
    Inventory inventory;
    int health;
    Vector2D position;

public:
    Player(Team team, Inventory inventory, int health, Vector2D position);

    bool is_tt() const;
    bool is_ct() const;
    bool has_bomb() const;
    bool has_prim_weapon() const;
    
    Inventory get_inventory() const;
    int get_health() const;
    float get_pos_x() const;
    float get_pos_y() const;

    void gain_money(int amount);
    
    void pick_bomb();

    void select_team(Team team);
    
    void buy_gun(const GunType& gun, int gun_price);
    
    void buy_ammo(const WeaponSlot& slot, int mag_price, int num_mags);
    
    void move(Vector2D step);

    ~Player();
};

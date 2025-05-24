#pragma once

#include <utility>

#include "common/game_state.h"
#include "common/models.h"
#include "server/clock/clock.h"
#include "server/utils/vector_2d.h"
#include "server/weapons/bomb.h"
#include "server/weapons/bullet.h"
#include "server/weapons/knife.h"

#include "inventory.h"

class Player {
private:
    Team team;
    Vector2D pos;
    bool ready;
    int health;
    int money;
    WeaponSlot current_weapon;
    Inventory inventory;

public:
    Player(Team team, Vector2D pos);

    bool is_ready() const;
    bool is_tt() const;
    bool is_ct() const;

    Vector2D get_pos() const;

    PlayerState state() const;

    void set_ready();

    // TODO: Player gain money after round finished
    // void gain_money(int amount);

    void pick_bomb();

    void select_team(Team team);

    void buy_gun(const GunType& gun, int gun_price);

    void buy_ammo(const WeaponSlot& slot, int mag_price, int num_mags);

    void move(Vector2D new_pos);

    void equip_weapon(WeaponSlot slot);

    void reload();

    ~Player();
};

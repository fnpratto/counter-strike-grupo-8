#pragma once

#include <utility>

#include "common/models.h"
#include "common/player_state.h"
#include "common/updates/player_update.h"
#include "server/clock/clock.h"
#include "server/utils/vector_2d.h"
#include "server/weapons/bomb.h"
#include "server/weapons/bullet.h"
#include "server/weapons/knife.h"

#include "inventory.h"

class Player {
private:
    PlayerState state;

public:
    Player(Team team, Vector2D pos);

    bool is_ready() const;
    bool is_tt() const;
    bool is_ct() const;
    bool is_moving() const;

    Vector2D get_pos() const;
    Vector2D get_move_dir() const;
    PlayerUpdate get_updates() const;

    void clear_updates();

    PlayerState get_state() const;

    void set_ready();

    // TODO: Player gain money after round finished
    void gain_money(int amount);

    void pick_bomb();

    void select_team(Team team);

    void buy_gun(const GunType& gun, int gun_price);

    void buy_ammo(const WeaponSlot& slot, int ammo_price, int num_mags);

    void start_moving(Vector2D velocity);

    void stop_moving();

    void move_to_pos(Vector2D new_pos);

    void aim(float x, float y);

    void equip_weapon(WeaponSlot slot);

    void reload();

    ~Player();
};

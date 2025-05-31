#pragma once

#include <utility>

#include "common/models.h"
#include "common/updates/player_update.h"
#include "server/clock/clock.h"
#include "server/logic.h"
#include "server/states/player_state.h"
#include "server/utils/vector_2d.h"
#include "server/weapons/bomb.h"
#include "server/weapons/bullet.h"
#include "server/weapons/knife.h"

#include "inventory.h"


class Player: public Logic<PlayerState, PlayerUpdate> {
public:
    Player(Team team, Vector2D pos);

    bool is_ready() const;
    bool is_tt() const;
    bool is_ct() const;
    bool is_moving() const;

    Vector2D get_pos() const;
    Vector2D get_move_dir() const;

    void set_ready();

    void select_team(Team team);

    // TODO: Player gain money after round finished
    void gain_money(int amount);
    void pick_bomb();

    void start_moving(Vector2D velocity);
    void stop_moving();
    void move_to_pos(Vector2D new_pos);

    void aim(float x, float y);
    void equip_weapon(WeaponSlot slot);

    Inventory& get_inventory();

    void reload();
};

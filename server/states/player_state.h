#pragma once

#include "common/models.h"
#include "common/updates/player_update.h"
#include "common/utils/vector_2d.h"
#include "server/player/inventory.h"

#include "state.h"

// Forward declaration
class Inventory;

class PlayerState: public State<PlayerUpdate> {
    Team team;
    Vector2D pos;
    Vector2D aim_direction;
    Vector2D velocity;
    bool ready;
    int health;
    WeaponSlot current_weapon;
    Inventory inventory;

public:
    PlayerState(Team team, Vector2D pos, Vector2D aim_direction, Vector2D velocity, bool ready,
                int health, WeaponSlot current_weapon):
            team(team),
            pos(pos),
            aim_direction(aim_direction),
            velocity(velocity),
            ready(ready),
            health(health),
            current_weapon(current_weapon) {
        updates = get_full_update();
    }

    Team get_team() const { return team; }
    Vector2D get_pos() const { return pos; }
    Vector2D get_aim_direction() const { return aim_direction; }
    Vector2D get_velocity() const { return velocity; }
    bool get_ready() const { return ready; }
    int get_health() const { return health; }
    WeaponSlot get_current_weapon() const { return current_weapon; }
    Inventory& get_inventory() { return inventory; }

    void set_team(Team new_team) {
        if (team == new_team)
            return;

        team = new_team;
        updates.set_team(new_team);
    }
    void set_pos(Vector2D new_pos) {
        if (pos == new_pos)
            return;

        pos = new_pos;
        updates.set_pos(new_pos);
    }
    void set_aim_direction(Vector2D new_aim_direction) {
        if (aim_direction == new_aim_direction)
            return;

        aim_direction = new_aim_direction;
        updates.set_aim_direction(new_aim_direction);
    }
    void set_velocity(Vector2D new_velocity) {
        if (velocity == new_velocity)
            return;

        velocity = new_velocity;
        updates.set_velocity(new_velocity);
    }
    void set_ready(bool new_ready) {
        if (ready == new_ready)
            return;

        ready = new_ready;
        updates.set_ready(new_ready);
    }
    void set_health(int new_health) {
        if (health == new_health)
            return;

        health = new_health;
        updates.set_health(new_health);
    }
    void set_current_weapon(WeaponSlot new_current_weapon) {
        if (current_weapon == new_current_weapon)
            return;

        current_weapon = new_current_weapon;
        updates.set_current_weapon(new_current_weapon);
    }

    void add_bomb() { inventory.add_bomb(); }

    PlayerUpdate get_updates() const override {
        PlayerUpdate update = updates;

        update.set_inventory(inventory.get_updates());

        return update;
    }

    PlayerUpdate get_full_update() const override {
        PlayerUpdate full_update;
        full_update.set_team(team);
        full_update.set_pos(pos);
        full_update.set_aim_direction(aim_direction);
        full_update.set_velocity(velocity);
        full_update.set_ready(ready);
        full_update.set_health(health);
        full_update.set_current_weapon(current_weapon);
        full_update.set_inventory(inventory.get_full_update());
        return full_update;
    }

    void clear_updates() override {
        State<PlayerUpdate>::clear_updates();
        inventory.clear_updates();
    }
};

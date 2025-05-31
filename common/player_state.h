#pragma once

#include "common/models.h"
#include "common/state.h"
#include "common/updates/player_update.h"
#include "server/player/inventory.h"
#include "server/utils/vector_2d.h"

// Forward declaration
class Inventory;

class PlayerState: public State<PlayerUpdate> {
    Team team;
    Vector2D pos;
    Vector2D aim_direction;
    Vector2D velocity;
    bool ready;
    int health;
    int money;
    WeaponSlot current_weapon;
    Inventory inventory;
    bool is_moving;

public:
    PlayerState() = default;

    // Delete copy constructor and copy assignment operator
    PlayerState(const PlayerState&) = delete;
    PlayerState& operator=(const PlayerState&) = delete;

    // Enable move constructor and move assignment operator
    PlayerState(PlayerState&&) = default;
    PlayerState& operator=(PlayerState&&) = default;

    PlayerState(Team team, Vector2D pos, Vector2D aim_direction, Vector2D velocity, bool ready,
                int health, int money, WeaponSlot current_weapon, bool is_moving):
            team(team),
            pos(pos),
            aim_direction(aim_direction),
            velocity(velocity),
            ready(ready),
            health(health),
            money(money),
            current_weapon(current_weapon),
            is_moving(is_moving) {}

    Team get_team() const { return team; }
    Vector2D get_pos() const { return pos; }
    Vector2D get_aim_direction() const { return aim_direction; }
    Vector2D get_velocity() const { return velocity; }
    bool get_ready() const { return ready; }
    int get_health() const { return health; }
    int get_money() const { return money; }
    WeaponSlot get_current_weapon() const { return current_weapon; }
    Inventory& get_inventory() { return inventory; }
    bool get_is_moving() const { return is_moving; }

    void set_team(Team new_team) {
        team = new_team;
        updates.set_team(new_team);
    }
    void set_pos(Vector2D new_pos) {
        pos = new_pos;
        updates.set_pos(new_pos);
    }
    void set_aim_direction(Vector2D new_aim_direction) {
        aim_direction = new_aim_direction;
        updates.set_aim_direction(new_aim_direction);
    }
    void set_velocity(Vector2D new_velocity) {
        velocity = new_velocity;
        updates.set_velocity(new_velocity);
    }
    void set_ready(bool new_ready) {
        ready = new_ready;
        updates.set_ready(new_ready);
    }
    void set_health(int new_health) {
        health = new_health;
        updates.set_health(new_health);
    }
    void set_money(int new_money) {
        money = new_money;
        updates.set_money(new_money);
    }
    void set_current_weapon(WeaponSlot new_current_weapon) {
        current_weapon = new_current_weapon;
        updates.set_current_weapon(new_current_weapon);
    }
    void set_is_moving(bool new_is_moving) {
        is_moving = new_is_moving;
        updates.set_is_moving(new_is_moving);
    }

    void add_bomb() { inventory.add_bomb(); }

    PlayerUpdate get_full_update() const override {
        PlayerUpdate full_update;
        full_update.set_team(team);
        full_update.set_pos(pos);
        full_update.set_aim_direction(aim_direction);
        full_update.set_velocity(velocity);
        full_update.set_ready(ready);
        full_update.set_health(health);
        full_update.set_money(money);
        full_update.set_current_weapon(current_weapon);
        full_update.set_is_moving(is_moving);
        full_update.set_inventory(inventory.get_full_update());
        return full_update;
    }
};

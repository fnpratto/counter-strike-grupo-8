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
                int health, WeaponSlot current_weapon);

    Team get_team() const;
    Vector2D get_pos() const;
    Vector2D get_aim_direction() const;
    Vector2D get_velocity() const;
    bool get_ready() const;
    int get_health() const;
    WeaponSlot get_current_weapon() const;
    Inventory& get_inventory();

    void set_team(Team new_team);
    void set_pos(Vector2D new_pos);
    void set_aim_direction(Vector2D new_aim_direction);
    void set_velocity(Vector2D new_velocity);
    void set_ready(bool new_ready);
    void set_health(int new_health);
    void set_current_weapon(WeaponSlot new_current_weapon);

    void add_bomb();

    PlayerUpdate get_updates() const override;
    PlayerUpdate get_full_update() const override;  // cppcheck-suppress[virtualCallInConstructor]

    void clear_updates() override;
};

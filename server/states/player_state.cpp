#include "player_state.h"

PlayerState::PlayerState(Team team, Vector2D pos, Vector2D aim_direction, Vector2D velocity,
                         bool ready, int health, WeaponSlot current_weapon):
        team(team),
        pos(pos),
        aim_direction(aim_direction),
        velocity(velocity),
        ready(ready),
        health(health),
        current_weapon(current_weapon) {
    updates = get_full_update();
}

Team PlayerState::get_team() const { return team; }

Vector2D PlayerState::get_pos() const { return pos; }

Vector2D PlayerState::get_aim_direction() const { return aim_direction; }

Vector2D PlayerState::get_velocity() const { return velocity; }

bool PlayerState::get_ready() const { return ready; }

int PlayerState::get_health() const { return health; }

WeaponSlot PlayerState::get_current_weapon() const { return current_weapon; }

Inventory& PlayerState::get_inventory() { return inventory; }

void PlayerState::set_team(Team new_team) {
    team = new_team;
    updates.set_team(new_team);
}

void PlayerState::set_pos(Vector2D new_pos) {
    pos = new_pos;
    updates.set_pos(new_pos);
}

void PlayerState::set_aim_direction(Vector2D new_aim_direction) {
    aim_direction = new_aim_direction;
    updates.set_aim_direction(new_aim_direction);
}

void PlayerState::set_velocity(Vector2D new_velocity) {
    velocity = new_velocity;
    updates.set_velocity(new_velocity);
}

void PlayerState::set_ready(bool new_ready) {
    ready = new_ready;
    updates.set_ready(new_ready);
}

void PlayerState::set_health(int new_health) {
    health = new_health;
    updates.set_health(new_health);
}

void PlayerState::set_current_weapon(WeaponSlot new_current_weapon) {
    current_weapon = new_current_weapon;
    updates.set_current_weapon(new_current_weapon);
}

void PlayerState::add_bomb() { inventory.add_bomb(); }

PlayerUpdate PlayerState::get_updates() const {
    PlayerUpdate update = updates;

    update.set_inventory(inventory.get_updates());

    return update;
}

PlayerUpdate PlayerState::get_full_update() const {
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

void PlayerState::clear_updates() {
    State<PlayerUpdate>::clear_updates();
    inventory.clear_updates();
}

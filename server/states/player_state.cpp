#include "player_state.h"

#include <utility>

PlayerState::PlayerState(Team team, Vector2D pos, Vector2D aim_direction, Vector2D velocity,
                         bool ready, int health, ItemSlot equipped_item):
        team(team),
        pos(pos),
        aim_direction(aim_direction),
        velocity(velocity),
        ready(ready),
        health(health),
        equipped_item(equipped_item) {
    updates = get_full_update();
}

Team PlayerState::get_team() const { return team; }

Vector2D PlayerState::get_pos() const { return pos; }

Vector2D PlayerState::get_aim_direction() const { return aim_direction; }

Vector2D PlayerState::get_velocity() const { return velocity; }

bool PlayerState::get_ready() const { return ready; }

int PlayerState::get_health() const { return health; }

ItemSlot PlayerState::get_equipped_item() const { return equipped_item; }

Inventory& PlayerState::get_inventory() { return inventory; }

void PlayerState::set_team(Team new_team) {
    if (team == new_team)
        return;
    team = new_team;
    updates.set_team(new_team);
}

void PlayerState::set_character_type(CharacterType new_character_type) {
    if (character_type == new_character_type)
        return;
    character_type = new_character_type;
    updates.set_character_type(new_character_type);
}

void PlayerState::set_pos(Vector2D new_pos) {
    if (pos == new_pos)
        return;
    pos = new_pos;
    updates.set_pos(new_pos);
}

void PlayerState::set_aim_direction(Vector2D new_aim_direction) {
    if (aim_direction == new_aim_direction)
        return;
    aim_direction = new_aim_direction;
    updates.set_aim_direction(new_aim_direction);
}

void PlayerState::set_velocity(Vector2D new_velocity) {
    if (velocity == new_velocity)
        return;
    velocity = new_velocity;
    updates.set_velocity(new_velocity);
}

void PlayerState::set_ready(bool new_ready) {
    if (ready == new_ready)
        return;
    ready = new_ready;
    updates.set_ready(new_ready);
}

void PlayerState::set_health(int new_health) {
    if (health == new_health)
        return;
    health = new_health;
    updates.set_health(new_health);
}

void PlayerState::set_equipped_item(ItemSlot new_equipped_item) {
    if (equipped_item == new_equipped_item)
        return;
    equipped_item = new_equipped_item;
    updates.set_equipped_item(new_equipped_item);
}

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
    full_update.set_equipped_item(equipped_item);
    full_update.set_inventory(inventory.get_full_update());
    return full_update;
}

void PlayerState::clear_updates() {
    State<PlayerUpdate>::clear_updates();
    inventory.clear_updates();
}

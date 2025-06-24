#include "player_state.h"

#include <utility>

PlayerState::PlayerState(Team team, CharacterType character_type, Circle hitbox,
                         Vector2D aim_direction, Vector2D velocity, bool ready,
                         ItemSlot equipped_item, const GameConfig::PlayerConfig& player_config,
                         const GameConfig::ItemsConfig& items_config):
        team(team),
        character_type(character_type),
        hitbox(hitbox),
        player_config(player_config),
        aim_direction(aim_direction),
        velocity(velocity),
        speed(player_config.speed),
        ready(ready),
        health(player_config.full_health),
        equipped_item(equipped_item),
        inventory(player_config.initial_money, items_config) {
    updates = get_full_update();
}

Team PlayerState::get_team() const { return team; }

CharacterType PlayerState::get_character_type() const { return character_type; }

Circle PlayerState::get_hitbox() const { return hitbox; }

GameConfig::PlayerConfig PlayerState::get_player_config() const { return player_config; }

Vector2D PlayerState::get_aim_direction() const { return aim_direction; }

Vector2D PlayerState::get_velocity() const { return velocity; }

int PlayerState::get_speed() const { return speed; }

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
    if (hitbox.center == new_pos)
        return;
    hitbox.center = new_pos;
    updates.set_pos(hitbox.center);
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
    full_update.set_character_type(character_type);
    full_update.set_pos(hitbox.center);
    full_update.set_hitbox_radius(hitbox.radius);
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

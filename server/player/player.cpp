#include "player.h"

#include <utility>

#include "common/models.h"
#include "server/errors.h"

#include "player_config.h"

Player::Player(Team team, Vector2D pos):
        Logic<PlayerState, PlayerUpdate>(
                PlayerState(team, pos, Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f), false,
                            PlayerConfig::full_health, ItemSlot::Secondary)) {}

bool Player::is_ready() const { return state.get_ready(); }

bool Player::is_tt() const { return state.get_team() == Team::TT; }

bool Player::is_ct() const { return state.get_team() == Team::CT; }

bool Player::is_moving() const { return state.get_velocity() != Vector2D(0.0f, 0.0f); }

bool Player::is_dead() const { return state.get_health() == 0; }

Vector2D Player::get_pos() const { return state.get_pos(); }

Vector2D Player::get_move_dir() const { return state.get_velocity(); }

Inventory& Player::get_inventory() { return state.get_inventory(); }

void Player::set_ready() { state.set_ready(true); }

void Player::take_damage(int damage) {
    int health = state.get_health();
    if (damage < health) {
        state.set_health(health - damage);
    } else {
        state.set_health(0);
        scoreboard_entry.deaths++;
    }
}

void Player::pick_bomb(Bomb&& bomb) { state.add_bomb(std::move(bomb)); }

void Player::select_team(Team team) { state.set_team(team); }

void Player::select_character(CharacterType character_type) {
    state.set_character_type(character_type);
}

void Player::start_moving(Vector2D velocity) { state.set_velocity(velocity); }

void Player::stop_moving() { state.set_velocity(Vector2D(0.0f, 0.0f)); }

void Player::move_to_pos(Vector2D new_pos) { state.set_pos(new_pos); }

void Player::aim(const Vector2D& direction) { state.set_aim_direction(direction); }

void Player::start_attacking() {
    ItemSlot slot = state.get_equipped_item();
    if (slot == ItemSlot::Melee) {
        auto& knife = state.get_inventory().get_knife();
        return knife.start_attacking();
    }
    if (slot == ItemSlot::Primary || slot == ItemSlot::Secondary) {
        auto& gun = state.get_inventory().get_gun(slot);
        return gun->start_attacking();
    }
}

std::vector<std::unique_ptr<AttackEffect>> Player::attack(TimePoint now) {
    ItemSlot slot = state.get_equipped_item();
    if (slot == ItemSlot::Melee) {
        auto& knife = state.get_inventory().get_knife();
        return knife.attack(*this, state.get_aim_direction(), now);
    }
    if (slot == ItemSlot::Primary || slot == ItemSlot::Secondary) {
        auto& gun = state.get_inventory().get_gun(slot);
        return gun->attack(*this, state.get_aim_direction(), now);
    }
    return {};
}

void Player::equip_item(ItemSlot slot) {
    if (!state.get_inventory().get_bomb().has_value() && slot == ItemSlot::Bomb)
        return;
    state.set_equipped_item(slot);
}

void Player::reload() {
    ItemSlot slot = state.get_equipped_item();
    if (slot != ItemSlot::Primary && slot != ItemSlot::Secondary)
        return;

    auto& gun = state.get_inventory().get_gun(slot);
    gun->reload();
}

void Player::increment_kills() { scoreboard_entry.kills++; }

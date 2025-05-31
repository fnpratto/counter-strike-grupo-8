#include "player.h"

#include <utility>

#include "common/models.h"
#include "server/errors.h"

#include "player_config.h"

Player::Player(Team team, Vector2D pos):
        state(team, pos, Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f), false,
              PlayerConfig::full_health, PlayerConfig::initial_money, WeaponSlot::Secondary,
              false) {}

bool Player::is_ready() const { return state.get_ready(); }

bool Player::is_tt() const { return state.get_team() == Team::TT; }

bool Player::is_ct() const { return state.get_team() == Team::CT; }

bool Player::is_moving() const { return state.get_is_moving(); }

Vector2D Player::get_pos() const { return state.get_pos(); }

Vector2D Player::get_move_dir() const { return state.get_velocity(); }

PlayerUpdate Player::get_updates() const { return state.get_updates(); }

void Player::clear_updates() { state.clear_updates(); }

PlayerState Player::get_state() const { return state; }

void Player::set_ready() { state.set_ready(true); }

void Player::gain_money(int amount) { state.set_money(state.get_money() + amount); }

// Logic -> State -> Update

void Player::pick_bomb() { state.add_bomb(); }

void Player::select_team(Team team) { state.set_team(team); }

void Player::start_moving(Vector2D velocity) { state.set_velocity(velocity); }

void Player::stop_moving() {
    if (state.get_is_moving()) {
        state.set_is_moving(false);
    }
}

void Player::move_to_pos(Vector2D new_pos) { state.set_pos(new_pos); }

void Player::aim(float x, float y) {
    Vector2D aim_dir = Vector2D(x, y).normalized();
    state.set_aim_direction(aim_dir);
}

void Player::equip_weapon(WeaponSlot slot) { state.set_current_weapon(slot); }

void Player::reload() {
    if (state.get_current_weapon() == WeaponSlot::Melee ||
        state.get_current_weapon() == WeaponSlot::Bomb)
        return;

    state.get_inventory().get_gun(state.get_current_weapon())->reload();
}

Player::~Player() {}

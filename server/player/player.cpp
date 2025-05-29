#include "player.h"

#include <utility>

#include "common/models.h"
#include "server/errors.h"

#include "player_config.h"

Player::Player(Team team, Vector2D pos):
        team(team),
        pos(pos),
        aim_dir(Vector2D(0, 0)),
        ready(false),
        health(PlayerConfig::full_health),
        money(PlayerConfig::initial_money),
        current_weapon(WeaponSlot::Secondary),
        state_is_moving(false),
        move_dir(Vector2D(0, 1)) {}

bool Player::is_ready() const { return ready; }

bool Player::is_tt() const { return team == Team::TT; }

bool Player::is_ct() const { return team == Team::CT; }

bool Player::is_moving() const { return state_is_moving; }

Vector2D Player::get_pos() const { return pos; }

Vector2D Player::get_move_dir() const { return move_dir; }

PlayerUpdate Player::get_updates() const { return updates; }

void Player::clear_updates() {
    updates.clear();
    inventory.clear_updates();
}

PlayerState Player::full_state() const {
    PlayerState player_state;
    player_state.team = team;
    player_state.pos_x = pos.get_x();
    player_state.pos_y = pos.get_y();
    player_state.aim_x = aim_dir.get_x();
    player_state.aim_y = aim_dir.get_y();
    player_state.ready = ready;
    player_state.health = health;
    player_state.money = money;
    player_state.current_weapon = current_weapon;
    player_state.inventory = inventory.full_state();
    player_state.is_moving = state_is_moving;
    return player_state;
}

void Player::set_ready() {
    ready = true;
    updates.set_ready(ready);
}

void Player::gain_money(int amount) {
    money += amount;
    updates.set_money(money);
}

void Player::pick_bomb() {
    inventory.add_bomb();
    updates.set_inventory(inventory.get_updates());
}

void Player::select_team(Team team) {
    this->team = team;
    updates.set_team(team);
}

void Player::buy_gun(const GunType& gun, int gun_price) {
    if (gun_price > money)
        throw BuyGunError();
    inventory.add_primary_weapon(gun);
    updates.set_inventory(inventory.get_updates());
    money -= gun_price;
    updates.set_money(money);
}

void Player::buy_ammo(const WeaponSlot& slot, int ammo_price, int num_mags) {
    if (ammo_price > money)
        throw BuyAmmoError();
    inventory.get_gun(slot)->add_mags(num_mags);
    updates.set_inventory(inventory.get_updates());
    money -= ammo_price;
    updates.set_money(money);
}

void Player::start_moving(int dx, int dy) {
    if (!state_is_moving) {
        state_is_moving = true;
        updates.set_is_moving(state_is_moving);
    }
    Vector2D new_move_dir(dx, dy);
    move_dir = new_move_dir;
    updates.set_move_dx(static_cast<int>(move_dir.get_x()));
    updates.set_move_dy(static_cast<int>(move_dir.get_y()));
}

void Player::stop_moving() {
    if (state_is_moving) {
        state_is_moving = false;
        updates.set_is_moving(state_is_moving);
    }
}

void Player::move_to_pos(Vector2D new_pos) {
    pos = new_pos;
    updates.set_pos_x(pos.get_x());
    updates.set_pos_y(pos.get_y());
}

void Player::aim(float x, float y) {
    aim_dir = Vector2D(x, y).normalized();
    updates.set_aim_x(aim_dir.get_x());
    updates.set_aim_y(aim_dir.get_y());
}

void Player::equip_weapon(WeaponSlot slot) {
    current_weapon = slot;
    updates.set_current_weapon(current_weapon);
}

void Player::reload() {
    if (current_weapon == WeaponSlot::Melee || current_weapon == WeaponSlot::Bomb)
        return;
    inventory.get_gun(current_weapon)->reload();
    updates.set_inventory(inventory.get_updates());
}

Player::~Player() {}

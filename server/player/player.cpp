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

PlayerState Player::state() const {
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
    player_state.inventory = inventory.state();
    return player_state;
}

void Player::set_ready() {
    ready = true;
    updates.add_change(PlayerAttr::READY, true);
}

// void Player::gain_money(int amount) {
//     inventory.add_money(amount);
// }

void Player::pick_bomb() {
    inventory.add_bomb();
    updates.add_change(PlayerAttr::INVENTORY, inventory.get_updates());
}

void Player::select_team(Team team) {
    this->team = team;
    updates.add_change(PlayerAttr::TEAM, team);
}

void Player::buy_gun(const GunType& gun, int gun_price) {
    if (gun_price > money)
        throw BuyGunError();
    inventory.add_primary_weapon(gun);
    updates.add_change(PlayerAttr::INVENTORY, inventory.get_updates());
    money += gun_price;
    updates.add_change(PlayerAttr::MONEY, money);
}

void Player::buy_ammo(const WeaponSlot& slot, int ammo_price, int num_mags) {
    if (ammo_price > money)
        throw BuyAmmoError();
    inventory.get_gun(slot)->add_mags(num_mags);
    updates.add_change(PlayerAttr::INVENTORY, inventory.get_updates());
    money -= ammo_price;
    updates.add_change(PlayerAttr::MONEY, money);
}

void Player::start_moving(int dx, int dy) {
    if (!state_is_moving) {
        state_is_moving = true;
        updates.add_change(PlayerAttr::IS_MOVING, true);
    }
    Vector2D new_move_dir(dx, dy);
    if (new_move_dir != move_dir) {
        move_dir = new_move_dir;
        updates.add_change(PlayerAttr::MOVE_DIR, move_dir);
    }
}

void Player::stop_moving() {
    if (state_is_moving) {
        state_is_moving = false;
        updates.add_change(PlayerAttr::IS_MOVING, false);
    }
}

void Player::move_to_pos(Vector2D new_pos) {
    pos = new_pos;
    updates.add_change(PlayerAttr::POS_X, pos.get_x());
    updates.add_change(PlayerAttr::POS_Y, pos.get_y());
}

void Player::aim(float x, float y) {
    aim_dir = Vector2D(x, y).normalized();
    updates.add_change(PlayerAttr::AIM_X, aim_dir.get_x());
    updates.add_change(PlayerAttr::AIM_Y, aim_dir.get_y());
}

void Player::equip_weapon(WeaponSlot slot) {
    current_weapon = slot;
    updates.add_change(PlayerAttr::CURRENT_WEAPON, current_weapon);
}

void Player::reload() {
    if (current_weapon == WeaponSlot::Melee || current_weapon == WeaponSlot::Bomb)
        return;
    inventory.get_gun(current_weapon)->reload();
    updates.add_change(PlayerAttr::INVENTORY, inventory.get_updates());
}

Player::~Player() {}

#include "player.h"

#include <utility>

#include "common/models.h"
#include "server/errors.h"

#include "player_config.h"

Player::Player(Team team, Vector2D pos):
        team(team),
        pos(pos),
        ready(false),
        health(PlayerConfig::full_health),
        money(PlayerConfig::initial_money),
        current_weapon(WeaponSlot::Secondary) {}

bool Player::is_ready() const { return ready; }

bool Player::is_tt() const { return team == Team::TT; }

bool Player::is_ct() const { return team == Team::CT; }

Vector2D Player::get_pos() const { return pos; }

PlayerState Player::state() const {
    PlayerState player_state;
    player_state.team = team;
    player_state.pos_x = pos.get_x();
    player_state.pos_y = pos.get_y();
    player_state.ready = ready;
    player_state.health = health;
    player_state.money = money;
    player_state.current_weapon = current_weapon;
    player_state.inventory = inventory.state();
    return player_state;
}

void Player::set_ready() { ready = true; }

// void Player::gain_money(int amount) {
//     inventory.add_money(amount);
// }

void Player::pick_bomb() { inventory.add_bomb(); }

void Player::select_team(Team team) { this->team = team; }

void Player::buy_gun(const GunType& gun, int gun_price) {
    if (gun_price > money)
        throw BuyGunError();
    inventory.add_primary_weapon(gun);
    money += gun_price;
}

void Player::buy_ammo(const WeaponSlot& slot, int ammo_price, int num_mags) {
    if (ammo_price > money)
        throw BuyAmmoError();
    inventory.get_gun(slot)->add_mags(num_mags);
    money -= ammo_price;
}

void Player::move(Vector2D new_pos) { pos = new_pos; }

void Player::equip_weapon(WeaponSlot slot) { current_weapon = slot; }

void Player::reload() {
    if (current_weapon == WeaponSlot::Melee || current_weapon == WeaponSlot::Bomb)
        return;
    inventory.get_gun(current_weapon)->reload();
}

Player::~Player() {}

#include "player.h"
#include "server/errors.h"

#define INIT_MONEY 800

Player::Player() : team(Team::Terrorist) {
    inventory.money = INIT_MONEY;
    inventory.weapons[WeaponSlot::Secondary] = WeaponType::Glock;
}

Team Player::get_team() const { return team; }

Inventory Player::get_inventory() const { return inventory; };

void Player::change_team(const Team& team) { this->team = team; };

void Player::gain_money(int amount) {
    inventory.money += amount;
}

void Player::buy_weapon(const WeaponType& weapon, int weapon_price) {
    if (weapon_price > inventory.money)
        throw BuyWeaponError();
    inventory.weapons[WeaponSlot::Primary] = weapon;
    inventory.money -= weapon_price;
}

Player::~Player() {}

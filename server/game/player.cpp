#include "player.h"
#include "server/errors.h"

Player::Player(Team team, Inventory inventory) : 
        team(team),
        inventory(inventory) {}

Team Player::get_team() const { return team; }

Inventory Player::get_inventory() const { return inventory; };

void Player::gain_money(int amount) {
    inventory.money += amount;
}

void Player::buy_weapon(const WeaponType& weapon, int weapon_price) {
    inventory.weapons[WeaponSlot::Primary] = weapon;
    inventory.money -= weapon_price;
}

Player::~Player() {}

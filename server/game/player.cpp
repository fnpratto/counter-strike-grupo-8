#include "player.h"
#include "server/errors.h"

Player::Player(Team team, Inventory inventory) : 
        team(team),
        inventory(inventory) {}

bool Player::is_terrorist() const {
    return team == Team::Terrorist;
}

bool Player::is_counter_terrorist() const {
    return team == Team::CounterTerrorist;
}

Inventory Player::get_inventory() const { return inventory; };

void Player::gain_money(int amount) {
    inventory.money += amount;
}

void Player::pick_bomb(WeaponType bomb) {
    inventory.weapons[WeaponSlot::Bomb] = bomb;
}

void Player::select_team(Team team) {
    this->team = team;
}

void Player::buy_weapon(const WeaponType& weapon, int weapon_price) {
    inventory.weapons[WeaponSlot::Primary] = weapon;
    inventory.money -= weapon_price;
}

Player::~Player() {}

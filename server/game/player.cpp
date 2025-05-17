#include "player.h"
#include "server/errors.h"

Player::Player(Team team, Inventory inventory, int health) : 
        team(team),
        inventory(inventory),
        health(health) {}

Inventory Player::get_inventory() const { return inventory; };

int Player::get_health() const { return health; }

bool Player::is_tt() const {
    return team == Team::Terrorist;
}

bool Player::is_ct() const {
    return team == Team::CounterTerrorist;
}

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

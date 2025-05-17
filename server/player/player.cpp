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
    inventory.add_money(amount);
}

void Player::pick_bomb() {
    inventory.add_bomb();
}

void Player::select_team(Team team) {
    this->team = team;
}

void Player::buy_gun(const GunType& weapon, int weapon_price) {
    int player_money = inventory.get_money();
    if (weapon_price > player_money)
        throw BuyGunError();
    inventory.add_primary_weapon(weapon);
    inventory.decrease_money(weapon_price);
}

void Player::buy_ammo(const WeaponSlot& slot, int mag_price, int num_mags) {
    int player_money = inventory.get_money();
    int total_ammo_price = mag_price * num_mags;
    if (total_ammo_price > player_money)
        throw BuyGunError();
    inventory.add_magazines(slot, num_mags);
    inventory.decrease_money(total_ammo_price);
}

Player::~Player() {}

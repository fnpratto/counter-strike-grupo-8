#include "player.h"
#include "server/errors.h"

Player::Player(Team team, Inventory inventory, int health, Vector2D position) : 
        team(team),
        inventory(inventory),
        health(health),
        position(position) {}

bool Player::is_tt() const { return team == Team::Terrorist; }
        
bool Player::is_ct() const { return team == Team::CounterTerrorist; }

bool Player::has_bomb() const { return inventory.has_bomb(); }

bool Player::has_prim_weapon() const { return inventory.has_prim_weapon(); }

Inventory Player::get_inventory() const { return inventory; };
        
int Player::get_health() const { return health; }

float Player::get_pos_x() const { return position.get_x(); }

float Player::get_pos_y() const { return position.get_y(); }

void Player::gain_money(int amount) {
    inventory.add_money(amount);
}

void Player::pick_bomb() {
    inventory.add_bomb();
}

void Player::select_team(Team team) {
    this->team = team;
}

void Player::buy_gun(const GunType& gun, int gun_price) {
    int player_money = inventory.get_money();
    if (gun_price > player_money)
        throw BuyGunError();
    inventory.add_primary_weapon(gun);
    inventory.decrease_money(gun_price);
}

void Player::buy_ammo(const WeaponSlot& slot, int ammo_price, int num_mags) {
    int player_money = inventory.get_money();
    if (ammo_price > player_money)
        throw BuyAmmoError();
    inventory.add_mags(slot, num_mags);
    inventory.decrease_money(ammo_price);
}

void Player::move(Vector2D step) {
    position = position + step;
}

Player::~Player() {}

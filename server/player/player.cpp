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

WeaponSlot Player::get_current_weapon() const { return current_weapon; }

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

void Player::equip_weapon(WeaponSlot slot) {
    current_weapon = slot;
}

Knife Player::attack() {
    std::unique_ptr<Utility> knife = inventory.get_melee_weapon();
    return *static_cast<Knife*>(knife.get());
}

std::vector<Bullet> Player::shoot_gun(int x, int y, TimePoint now) {
    std::unique_ptr<Gun> gun = nullptr;
    if (current_weapon == WeaponSlot::Primary) {
        gun = inventory.get_prim_weapon();
    } else {
        gun = inventory.get_sec_weapon();
    }
    return gun->shoot(position, Vector2D(x, y), now);
}

Player::~Player() {}

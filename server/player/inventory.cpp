#include "inventory.h"

#include <map>

#include "common/models.h"
#include "server/cons.h"
#include "server/weapons/glock.h"
#include "server/weapons/knife.h"
#include "server/weapons/bomb.h"
#include "server/weapons/ak47.h"
#include "server/weapons/m3.h"
#include "server/weapons/awp.h"

Inventory::Inventory() : money(PlayerInitialConfig::initial_money) {
    guns[WeaponSlot::Secondary] = std::make_unique<Glock>();
    utilities[WeaponSlot::Melee] = std::make_unique<Knife>();
}

Inventory::Inventory(const Inventory& other) : money(other.money) {
    for (const auto& [slot, gun] : other.guns) {
        guns[slot] = gun->clone();
    }
    for (const auto& [slot, utility] : other.utilities) {
        utilities[slot] = utility->clone();
    }
}

bool Inventory::has_bomb() const { 
    return utilities.find(WeaponSlot::Bomb) != utilities.end();
}

bool Inventory::has_prim_weapon() const {
    return guns.find(WeaponSlot::Primary) != guns.end();
}

int Inventory::get_money() const { return money; }

std::unique_ptr<Gun> Inventory::get_prim_weapon() const {
    return guns.at(WeaponSlot::Primary)->clone();
}

std::unique_ptr<Gun> Inventory::get_sec_weapon() const {
    return guns.at(WeaponSlot::Secondary)->clone();
}

std::unique_ptr<Utility> Inventory::get_melee_weapon() const {
    return utilities.at(WeaponSlot::Melee)->clone();
}

std::unique_ptr<Utility> Inventory::get_bomb() const {
    return utilities.at(WeaponSlot::Bomb)->clone();
}

void Inventory::add_money(int amount) { money += amount; }

void Inventory::decrease_money(int amount) { money -= amount; }

void Inventory::add_bomb() {
    utilities[WeaponSlot::Bomb] = std::make_unique<Bomb>();
}

void Inventory::add_primary_weapon(const GunType& weapon_type) {
    if (weapon_type == GunType::AK47) {
        guns[WeaponSlot::Primary] = std::make_unique<Ak47>();
    } else if (weapon_type == GunType::M3) {
        guns[WeaponSlot::Primary] = std::make_unique<M3>();
    } else if (weapon_type == GunType::AWP) {
        guns[WeaponSlot::Primary] = std::make_unique<Awp>();
    }
}

void Inventory::add_mags(const WeaponSlot& slot, int num_mags) {
    guns.at(slot)->add_mags(num_mags);
}

Inventory::~Inventory() {}

Inventory& Inventory::operator=(const Inventory& other) {
    if (this == &other) return *this;

    money = other.money;

    guns.clear();
    for (const auto& [slot, gun] : other.guns) {
        guns[slot] = gun->clone();
    }

    utilities.clear();
    for (const auto& [slot, utility] : other.utilities) {
        utilities[slot] = utility->clone();
    }

    return *this;
}

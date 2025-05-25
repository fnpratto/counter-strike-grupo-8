#include "inventory.h"

#include <map>

#include "common/models.h"
#include "server/weapons/ak47.h"
#include "server/weapons/awp.h"
#include "server/weapons/bomb.h"
#include "server/weapons/glock.h"
#include "server/weapons/knife.h"
#include "server/weapons/m3.h"

Inventory::Inventory() {
    guns[WeaponSlot::Secondary] = std::make_unique<Glock>();
    utilities[WeaponSlot::Melee] = std::make_unique<Knife>();
}

std::unique_ptr<Gun>& Inventory::get_gun(const WeaponSlot& slot) { return guns.at(slot); }

InventoryUpdate Inventory::get_updates() const { return updates; }

InventoryState Inventory::state() const {
    InventoryState inventory_state;

    std::map<WeaponSlot, GunState> guns_states;
    for (auto& [slot, gun]: guns) {
        guns_states[slot] = gun->state();
    }
    inventory_state.guns = guns_states;

    std::map<WeaponSlot, UtilityState> utilities_states;
    for (auto& [slot, util]: utilities) {
        utilities_states[slot] = util->state();
    }
    inventory_state.utilities = utilities_states;

    return inventory_state;
}

void Inventory::add_bomb() {
    utilities[WeaponSlot::Bomb] = std::make_unique<Bomb>();

    std::map<WeaponSlot, UtilityType> utility_update;
    utility_update.emplace(WeaponSlot::Bomb, utilities[WeaponSlot::Bomb]->get_type());
    updates.add_change(InventoryAttr::UTILITIES, utility_update);
}

void Inventory::add_primary_weapon(const GunType& weapon_type) {
    if (weapon_type == GunType::AK47) {
        guns[WeaponSlot::Primary] = std::make_unique<Ak47>();
    } else if (weapon_type == GunType::M3) {
        guns[WeaponSlot::Primary] = std::make_unique<M3>();
    } else if (weapon_type == GunType::AWP) {
        guns[WeaponSlot::Primary] = std::make_unique<Awp>();
    }

    std::map<WeaponSlot, GunUpdate> gun_updates;
    gun_updates.emplace(WeaponSlot::Primary, guns[WeaponSlot::Primary]->get_updates());
    updates.add_change(InventoryAttr::GUNS, gun_updates);
}

Inventory::~Inventory() {}

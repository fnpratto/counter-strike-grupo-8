#include "inventory.h"

#include <map>

#include "common/models.h"
#include "server/weapons/ak47.h"
#include "server/weapons/awp.h"
#include "server/weapons/bomb.h"
#include "server/weapons/glock.h"
#include "server/weapons/knife.h"
#include "server/weapons/m3.h"

const std::unique_ptr<Gun>& Inventory::get_gun(const WeaponSlot& slot) const {
    return state.get_guns().at(slot);
}

void Inventory::add_bomb() { state.set_utility(WeaponSlot::Bomb, std::make_unique<Bomb>()); }

void Inventory::add_primary_weapon(const GunType& weapon_type) {
    if (weapon_type == GunType::AK47) {
        state.set_gun(WeaponSlot::Primary, std::make_unique<Ak47>());
    } else if (weapon_type == GunType::M3) {
        state.set_gun(WeaponSlot::Primary, std::make_unique<M3>());
    } else if (weapon_type == GunType::AWP) {
        state.set_gun(WeaponSlot::Primary, std::make_unique<Awp>());
    }
}

#include "inventory_state.h"

#include "server/weapons/glock.h"
#include "server/weapons/knife.h"

InventoryState::InventoryState(): money(0) {
    guns[WeaponSlot::Secondary] = std::make_unique<Glock>();
    utilities[WeaponSlot::Melee] = std::make_unique<Knife>();
}

#pragma once

#include <map>
#include <memory>

#include "common/models.h"
#include "common/updates/inventory_update.h"
#include "server/logic.h"
#include "server/states/inventory_state.h"
#include "server/weapons/gun.h"
#include "server/weapons/utility.h"


class Inventory: public Logic<InventoryState, InventoryUpdate> {
public:
    Inventory(): Logic<InventoryState, InventoryUpdate>(InventoryState()) {}

    // TODO is this needed?
    // Delete copy constructor and copy assignment operator
    Inventory(const Inventory&) = delete;
    Inventory& operator=(const Inventory&) = delete;

    // Enable move constructor and move assignment operator
    Inventory(Inventory&&) = default;
    Inventory& operator=(Inventory&&) = default;

    void add_bomb();
    void add_primary_weapon(const GunType& weapon_type);
    int get_money() const { return state.get_money(); }
    void set_money(int new_money) { state.set_money(new_money); }

    const std::unique_ptr<Gun>& get_gun(const WeaponSlot& slot) const;
};

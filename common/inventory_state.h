#pragma once

#include <map>
#include <memory>

#include "common/models.h"
#include "common/state.h"
#include "common/updates/inventory_update.h"
#include "server/weapons/gun.h"
#include "server/weapons/utility.h"

class InventoryState: public State<InventoryUpdate> {
    std::map<WeaponSlot, std::unique_ptr<Gun>> guns;
    std::map<WeaponSlot, std::unique_ptr<Utility>> utilities;

public:
    InventoryState();

    const std::map<WeaponSlot, std::unique_ptr<Gun>>& get_guns() const { return guns; }
    const std::map<WeaponSlot, std::unique_ptr<Utility>>& get_utilities() const {
        return utilities;
    }

    void set_gun(WeaponSlot slot, std::unique_ptr<Gun>&& gun) {
        guns[slot] = std::move(gun);
        updates.add_guns_change(slot, guns.at(slot)->get_updates());
    }

    void set_utility(WeaponSlot slot, std::unique_ptr<Utility>&& utility) {
        utilities[slot] = std::move(utility);
    }

    friend class Inventory;  // Allow Inventory to access private members
};

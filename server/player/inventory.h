#pragma once

#include <map>
#include <memory>

#include "common/inventory_state.h"
#include "common/models.h"
#include "common/updates/inventory_update.h"
#include "server/weapons/gun.h"
#include "server/weapons/utility.h"

class Inventory {
private:
    InventoryState state;

public:
    void add_bomb();
    void add_primary_weapon(const GunType& weapon_type);

    InventoryState get_state() const;
    const std::unique_ptr<Gun>& get_gun(const WeaponSlot& slot) const;
    InventoryUpdate get_updates() const;

    void clear_updates();

    ~Inventory();
};

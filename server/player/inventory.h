#pragma once

#include <map>
#include <memory>

#include "common/game_state.h"
#include "common/game_state_update.h"
#include "common/models.h"
#include "server/weapons/gun.h"
#include "server/weapons/utility.h"

class Inventory {
private:
    std::map<WeaponSlot, std::unique_ptr<Gun>> guns;
    std::map<WeaponSlot, std::unique_ptr<Utility>> utilities;
    InventoryUpdate updates;

public:
    Inventory();

    std::unique_ptr<Gun>& get_gun(const WeaponSlot& slot);

    InventoryUpdate get_updates() const;

    InventoryState state() const;

    void add_bomb();
    void add_primary_weapon(const GunType& weapon_type);

    ~Inventory();
};

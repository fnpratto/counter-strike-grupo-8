#pragma once

#include <memory>
#include <map>

#include "common/models.h"
#include "server/weapons/gun.h"
#include "server/weapons/utility.h"
#include "common/game_state.h"

class Inventory {
private:
    std::map<WeaponSlot, std::unique_ptr<Gun>> guns;
    std::map<WeaponSlot, std::unique_ptr<Utility>> utilities;

public:
    Inventory();

    std::unique_ptr<Gun>& get_gun(const WeaponSlot& slot);

    InventoryState state() const;
    
    void add_bomb();
    void add_primary_weapon(const GunType& weapon_type);
    
    ~Inventory();
};

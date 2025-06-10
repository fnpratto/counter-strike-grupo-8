#pragma once

#include <map>
#include <memory>
#include <optional>

#include "common/models.h"
#include "common/updates/inventory_update.h"
#include "server/logic.h"
#include "server/player/player_config.h"
#include "server/states/inventory_state.h"
#include "server/weapons/gun.h"
#include "server/weapons/knife.h"

class Inventory: public Logic<InventoryState, InventoryUpdate> {
public:
    Inventory():
            Logic<InventoryState, InventoryUpdate>(InventoryState(PlayerConfig::initial_money)) {
        state.set_gun(ItemSlot::Secondary, Gun::make_glock());
    }

    // TODO is this needed?
    // Delete copy constructor and copy assignment operator
    Inventory(const Inventory&) = delete;
    Inventory& operator=(const Inventory&) = delete;

    // Enable move constructor and move assignment operator
    Inventory(Inventory&&) = default;
    Inventory& operator=(Inventory&&) = default;

    int get_money() const { return state.get_money(); }
    std::unique_ptr<Gun>& get_gun(const ItemSlot& slot);
    Knife& get_knife();
    std::optional<Bomb>& get_bomb();

    void set_money(int new_money) { state.set_money(new_money); }
    void add_primary_weapon(const GunType& weapon_type);
    void add_bomb(Bomb&& bomb);
};

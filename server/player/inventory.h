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
    Inventory();

    Inventory(const Inventory&) = delete;
    Inventory& operator=(const Inventory&) = delete;

    Inventory(Inventory&&) = default;
    Inventory& operator=(Inventory&&) = default;

    bool has_item_in_slot(ItemSlot slot);

    int get_money() const;
    std::map<ItemSlot, std::unique_ptr<Gun>>& get_guns();
    Knife& get_knife();
    std::optional<Bomb>& get_bomb();

    void set_money(int new_money) { state.set_money(new_money); }

    void set_gun(std::unique_ptr<Gun>&& gun);
    void add_primary_weapon(const GunType& weapon_type);
    std::unique_ptr<Gun> remove_primary_weapon();

    void set_bomb(Bomb&& bomb);
    Bomb remove_bomb();
};

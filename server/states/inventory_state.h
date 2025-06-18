#pragma once

#include <map>
#include <memory>
#include <optional>
#include <utility>

#include "common/models.h"
#include "common/updates/inventory_update.h"
#include "server/items/bomb.h"
#include "server/items/gun.h"
#include "server/items/knife.h"

#include "state.h"

class InventoryState: public State<InventoryUpdate> {
    int money;
    std::map<ItemSlot, std::unique_ptr<Gun>> guns;
    Knife knife;
    std::optional<Bomb> bomb;

public:
    explicit InventoryState(int money);

    int get_money() const;
    std::map<ItemSlot, std::unique_ptr<Gun>>& get_guns();
    Knife& get_knife();
    std::optional<Bomb>& get_bomb();

    void set_money(int new_money);
    void set_gun(ItemSlot slot, std::unique_ptr<Gun>&& gun);
    void set_bomb(Bomb&& bomb);

    InventoryUpdate get_updates() const override;

    InventoryUpdate get_full_update()  // cppcheck-suppress[virtualCallInConstructor]
            const override;

    void clear_updates() override;
};

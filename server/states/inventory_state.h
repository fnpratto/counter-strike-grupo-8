#pragma once

#include <map>
#include <memory>
#include <utility>

#include "common/models.h"
#include "common/updates/inventory_update.h"
#include "server/weapons/gun.h"
#include "server/weapons/utility.h"

#include "state.h"

class InventoryState: public State<InventoryUpdate> {
    int money;
    std::map<WeaponSlot, std::unique_ptr<Gun>> guns;
    std::map<WeaponSlot, std::unique_ptr<Utility>> utilities;

public:
    explicit InventoryState(int money);

    int get_money() const;
    const std::map<WeaponSlot, std::unique_ptr<Gun>>& get_guns() const;
    const std::map<WeaponSlot, std::unique_ptr<Utility>>& get_utilities() const;

    void set_money(int new_money);
    void set_gun(WeaponSlot slot, std::unique_ptr<Gun>&& gun);
    void set_utility(WeaponSlot slot, std::unique_ptr<Utility>&& utility);

    InventoryUpdate get_updates() const override;
    InventoryUpdate get_full_update()  // cppcheck-suppress[virtualCallInConstructor]
            const override;

    void clear_updates() override;
};

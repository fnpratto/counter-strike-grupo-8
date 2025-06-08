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
    explicit InventoryState(int money): money(money) { updates = get_full_update(); }

    void set_money(int new_money) {
        if (money == new_money)
            return;
        money = new_money;
        updates.set_money(new_money);
    }
    int get_money() const { return money; }

    const std::map<WeaponSlot, std::unique_ptr<Gun>>& get_guns() const { return guns; }
    const std::map<WeaponSlot, std::unique_ptr<Utility>>& get_utilities() const {
        return utilities;
    }

    void set_gun(WeaponSlot slot, std::unique_ptr<Gun>&& gun) { guns[slot] = std::move(gun); }

    void set_utility(WeaponSlot slot, std::unique_ptr<Utility>&& utility) {
        utilities[slot] = std::move(utility);
    }

    InventoryUpdate get_updates() const override {
        InventoryUpdate update = updates;

        for (const auto& [slot, gun]: guns) update.add_guns_change(slot, gun->get_updates());

        for (const auto& [slot, utility]: utilities)
            update.add_utilities_change(slot, utility->get_updates());

        return update;
    }

    InventoryUpdate get_full_update() const override {
        InventoryUpdate full_update;
        full_update.set_money(money);
        for (const auto& [slot, gun]: guns) {
            full_update.add_guns_change(slot, gun->get_full_update());
        }
        for (const auto& [slot, utility]: utilities) {
            full_update.add_utilities_change(slot, utility->get_full_update());
        }
        return full_update;
    }

    void clear_updates() override {
        State<InventoryUpdate>::clear_updates();
        for (auto& [_, gun]: guns) gun->clear_updates();  // cppcheck-suppress[unusedVariable]
        for (auto& [_, utility]: utilities)               // cppcheck-suppress[unusedVariable]
            utility->clear_updates();
    }
};

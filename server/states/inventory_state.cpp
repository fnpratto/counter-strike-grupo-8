#include "inventory_state.h"

InventoryState::InventoryState(int money): money(money) { updates = get_full_update(); }

int InventoryState::get_money() const { return money; }

const std::map<WeaponSlot, std::unique_ptr<Gun>>& InventoryState::get_guns() const { return guns; }

const std::map<WeaponSlot, std::unique_ptr<Utility>>& InventoryState::get_utilities() const {
    return utilities;
}

void InventoryState::set_money(int new_money) {
    if (money == new_money)
        return;
    money = new_money;
    updates.set_money(new_money);
}

void InventoryState::set_gun(WeaponSlot slot, std::unique_ptr<Gun>&& gun) {
    guns[slot] = std::move(gun);
}

void InventoryState::set_utility(WeaponSlot slot, std::unique_ptr<Utility>&& utility) {
    utilities[slot] = std::move(utility);
}

InventoryUpdate InventoryState::get_updates() const {
    InventoryUpdate update = updates;

    for (const auto& [slot, gun]: guns) update.add_guns_change(slot, gun->get_updates());

    for (const auto& [slot, utility]: utilities)
        update.add_utilities_change(slot, utility->get_updates());

    return update;
}

InventoryUpdate InventoryState::get_full_update() const {
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

void InventoryState::clear_updates() {
    State<InventoryUpdate>::clear_updates();
    for (auto& [_, gun]: guns) gun->clear_updates();  // cppcheck-suppress[unusedVariable]
    for (auto& [_, utility]: utilities)               // cppcheck-suppress[unusedVariable]
        utility->clear_updates();
}

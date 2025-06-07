#include "inventory_state.h"

#include <vector>

InventoryState::InventoryState(int money): money(money) { updates = get_full_update(); }

int InventoryState::get_money() const { return money; }

std::map<ItemSlot, std::unique_ptr<Gun>>& InventoryState::get_guns() { return guns; }

Knife& InventoryState::get_knife() { return knife; }

std::optional<Bomb>& InventoryState::get_bomb() { return bomb; }

void InventoryState::set_money(int new_money) {
    if (money == new_money)
        return;
    money = new_money;
    updates.set_money(new_money);
}

void InventoryState::set_gun(ItemSlot slot, std::unique_ptr<Gun>&& gun) {
    guns[slot] = std::move(gun);
    updates.set_weapons_added(std::vector<ItemSlot>{slot});
}

void InventoryState::set_bomb(Bomb&& bomb) {
    this->bomb = bomb;
    updates.set_weapons_added(std::vector<ItemSlot>{ItemSlot::Bomb});
}

InventoryUpdate InventoryState::get_updates() const {
    InventoryUpdate update = updates;
    for (const auto& [slot, gun]: guns) update.add_guns_change(slot, gun->get_updates());
    return update;
}

InventoryUpdate InventoryState::get_full_update() const {
    InventoryUpdate full_update;
    std::vector<ItemSlot> weapons_added;

    full_update.set_money(money);
    for (const auto& [slot, gun]: guns) {
        full_update.add_guns_change(slot, gun->get_full_update());
        weapons_added.push_back(slot);
    }
    weapons_added.push_back(ItemSlot::Melee);
    if (bomb.has_value())
        weapons_added.push_back(ItemSlot::Bomb);
    full_update.set_weapons_added(std::move(weapons_added));

    return full_update;
}

void InventoryState::clear_updates() {
    State<InventoryUpdate>::clear_updates();
    for (auto& [_, gun]: guns) gun->clear_updates();  // cppcheck-suppress[unusedVariable]
}

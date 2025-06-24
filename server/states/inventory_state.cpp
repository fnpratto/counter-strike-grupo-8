#include "inventory_state.h"

#include <vector>

InventoryState::InventoryState(int money, const GameConfig::ItemsConfig::KnifeConfig& knife_config):
        money(money), knife(knife_config) {
    updates = get_full_update();
}

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
}

void InventoryState::set_bomb(Bomb&& bomb) { this->bomb = std::move(bomb); }

std::unique_ptr<Gun> InventoryState::remove_primary_weapon() {
    auto gun = std::move(guns.at(ItemSlot::Primary));
    guns.erase(ItemSlot::Primary);
    return gun;
}

Bomb InventoryState::remove_bomb() {
    Bomb removed_bomb = std::move(bomb.value());
    bomb.reset();
    updates.set_bomb(std::optional<BombUpdate>());
    return removed_bomb;
}

InventoryUpdate InventoryState::get_updates() const {
    InventoryUpdate update = updates;
    for (const auto& [slot, gun]: guns) update.add_guns_change(slot, gun->get_updates());
    update.set_knife(knife.get_updates());
    if (bomb.has_value())
        update.set_bomb(bomb->get_updates());
    return update;
}

InventoryUpdate InventoryState::get_full_update() const {
    InventoryUpdate full_update;

    full_update.set_money(money);
    for (const auto& [slot, gun]: guns) {
        full_update.add_guns_change(slot, gun->get_full_update());
    }
    full_update.set_knife(knife.get_full_update());
    if (bomb.has_value())
        full_update.set_bomb(bomb->get_full_update());
    else
        full_update.set_bomb(std::optional<BombUpdate>());

    return full_update;
}

void InventoryState::clear_updates() {
    State<InventoryUpdate>::clear_updates();
    for (auto& [_, gun]: guns) gun->clear_updates();  // cppcheck-suppress[unusedVariable]
    knife.clear_updates();
    if (bomb.has_value())
        bomb.value().clear_updates();
}

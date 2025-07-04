#include "inventory.h"

#include <map>
#include <utility>

#include "common/models.h"
#include "server/items/knife.h"

Inventory::Inventory(int initial_money, const GameConfig::ItemsConfig& items_config):
        Logic<InventoryState, InventoryUpdate>(InventoryState(initial_money, items_config.knife)),
        items_config(items_config) {
    state.set_gun(ItemSlot::Secondary, std::make_unique<Gun>(GunType::Glock, items_config.glock));
}

bool Inventory::has_item_in_slot(ItemSlot slot) {
    switch (slot) {
        case ItemSlot::Primary:
            return state.get_guns().find(ItemSlot::Primary) != state.get_guns().end();
        case ItemSlot::Secondary:
            return state.get_guns().find(ItemSlot::Secondary) != state.get_guns().end();
        case ItemSlot::Melee:
            return true;
        case ItemSlot::Bomb:
            return state.get_bomb().has_value();
        default:
            return false;
    }
}

int Inventory::get_money() const { return state.get_money(); }

std::map<ItemSlot, std::unique_ptr<Gun>>& Inventory::get_guns() { return state.get_guns(); }

Knife& Inventory::get_knife() { return state.get_knife(); }

std::optional<Bomb>& Inventory::get_bomb() { return state.get_bomb(); }

void Inventory::set_money(int new_money) { state.set_money(new_money); }

void Inventory::set_gun(std::unique_ptr<Gun>&& gun) {
    if (gun->get_type() == GunType::Glock) {
        state.set_gun(ItemSlot::Secondary, std::move(gun));
    } else if (gun->get_type() == GunType::AK47 || gun->get_type() == GunType::M3 ||
               gun->get_type() == GunType::AWP) {
        state.set_gun(ItemSlot::Primary, std::move(gun));
    }
}

std::unique_ptr<Gun> Inventory::remove_primary_weapon() { return state.remove_primary_weapon(); }

Bomb Inventory::remove_bomb() { return state.remove_bomb(); }

void Inventory::set_bomb(Bomb&& bomb) { state.set_bomb(std::move(bomb)); }

#include "inventory.h"

#include <map>
#include <utility>

#include "common/models.h"
#include "server/weapons/knife.h"

Inventory::Inventory():
        Logic<InventoryState, InventoryUpdate>(InventoryState(PlayerConfig::initial_money)) {
    state.set_gun(ItemSlot::Secondary, Gun::make_glock());
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

void Inventory::add_primary_weapon(const GunType& gun_type) {
    if (gun_type == GunType::AK47) {
        state.set_gun(ItemSlot::Primary, Gun::make_ak47());
    } else if (gun_type == GunType::M3) {
        state.set_gun(ItemSlot::Primary, Gun::make_m3());
    } else if (gun_type == GunType::AWP) {
        state.set_gun(ItemSlot::Primary, Gun::make_awp());
    }
}

std::unique_ptr<Gun> Inventory::remove_primary_weapon() {
    auto gun = std::move(get_guns().at(ItemSlot::Primary));
    state.get_guns().erase(ItemSlot::Primary);
    return gun;
}

Bomb Inventory::remove_bomb() {
    auto bomb = std::move(get_bomb().value());
    state.get_bomb().reset();
    return bomb;
}

void Inventory::add_bomb(Bomb&& bomb) { state.set_bomb(std::move(bomb)); }

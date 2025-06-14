#include "inventory.h"

#include <map>
#include <utility>

#include "common/models.h"
#include "server/weapons/knife.h"

Inventory::Inventory():
        Logic<InventoryState, InventoryUpdate>(InventoryState(PlayerConfig::initial_money)) {
    state.set_gun(ItemSlot::Secondary, Gun::make_glock());
}

std::map<ItemSlot, std::unique_ptr<Gun>>& Inventory::get_guns() { return state.get_guns(); }

Knife& Inventory::get_knife() { return state.get_knife(); }

std::optional<Bomb>& Inventory::get_bomb() { return state.get_bomb(); }

void Inventory::add_bomb(Bomb&& bomb) { state.set_bomb(std::move(bomb)); }

void Inventory::add_primary_weapon(const GunType& gun_type) {
    if (gun_type == GunType::AK47) {
        state.set_gun(ItemSlot::Primary, Gun::make_ak47());
    } else if (gun_type == GunType::M3) {
        state.set_gun(ItemSlot::Primary, Gun::make_m3());
    } else if (gun_type == GunType::AWP) {
        state.set_gun(ItemSlot::Primary, Gun::make_awp());
    }
}

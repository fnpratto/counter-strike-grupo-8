#include "inventory.h"

#include <map>
#include <utility>

#include "common/models.h"
#include "server/weapons/knife.h"

std::unique_ptr<Gun>& Inventory::get_gun(const ItemSlot& slot) { return state.get_guns().at(slot); }

Knife& Inventory::get_knife() { return state.get_knife(); }

std::optional<Bomb>& Inventory::get_bomb() { return state.get_bomb(); }

void Inventory::add_bomb(Bomb&& bomb) { state.set_bomb(std::move(bomb)); }

void Inventory::add_primary_weapon(const GunType& gun_type) {
    if (gun_type == GunType::AK47) {
        state.set_gun(ItemSlot::Primary, Gun::ak47());
    } else if (gun_type == GunType::M3) {
        state.set_gun(ItemSlot::Primary, Gun::m3());
    } else if (gun_type == GunType::AWP) {
        state.set_gun(ItemSlot::Primary, Gun::awp());
    }
}

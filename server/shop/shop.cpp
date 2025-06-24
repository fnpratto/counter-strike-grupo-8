#include "shop.h"

#include <memory>

#include "server/items/gun.h"

#include "shop_prices.h"

Shop::Shop() {
    gun_prices[GunType::AK47] = ShopPrices::ak47;
    gun_prices[GunType::M3] = ShopPrices::m3;
    gun_prices[GunType::AWP] = ShopPrices::awp;

    ammo_prices[GunType::AK47] = ShopPrices::mag_ak47;
    ammo_prices[GunType::M3] = ShopPrices::mag_m3;
    ammo_prices[GunType::AWP] = ShopPrices::mag_awp;
    ammo_prices[GunType::Glock] = ShopPrices::mag_glock;
}

std::map<GunType, int> Shop::get_gun_prices() const { return gun_prices; }

std::map<GunType, int> Shop::get_ammo_prices() const { return ammo_prices; }

bool Shop::can_buy_gun(const GunType& gun_type, Inventory& inventory) const {
    if (gun_type == GunType::Glock)
        return false;
    return gun_prices.at(gun_type) <= inventory.get_money();
}

bool Shop::can_buy_ammo(const ItemSlot& slot, Inventory& inventory) const {
    if (slot != ItemSlot::Primary && slot != ItemSlot::Secondary)
        return false;

    auto& gun = inventory.get_guns().at(slot);
    GunType gun_type = gun->get_type();
    return ammo_prices.at(gun_type) <= inventory.get_money();
}

void Shop::buy_gun(const GunType& gun_type, Inventory& inventory) const {
    if (!can_buy_gun(gun_type, inventory))
        return;
    inventory.set_gun(Gun::make_gun(gun_type));
    inventory.set_money(inventory.get_money() - gun_prices.at(gun_type));
}

void Shop::buy_ammo(const ItemSlot& slot, Inventory& inventory) const {
    if (!can_buy_ammo(slot, inventory))
        return;
    auto& gun = inventory.get_guns().at(slot);
    GunType gun_type = gun->get_type();
    gun->add_mag();
    inventory.set_money(inventory.get_money() - ammo_prices.at(gun_type));
}

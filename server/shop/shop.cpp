#include "shop.h"

#include <memory>

#include "server/items/gun.h"

Shop::Shop(const GameConfig::ShopPrices& prices) {
    gun_prices[GunType::AK47] = prices.ak47;
    gun_prices[GunType::M3] = prices.m3;
    gun_prices[GunType::AWP] = prices.awp;

    ammo_prices[GunType::AK47] = prices.mag_ak47;
    ammo_prices[GunType::M3] = prices.mag_m3;
    ammo_prices[GunType::AWP] = prices.mag_awp;
    ammo_prices[GunType::Glock] = prices.mag_glock;
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

void Shop::buy_gun(const GunType& gun_type, Inventory& inventory,
                   const GameConfig::ItemsConfig::GunConfig& gun_config) const {
    if (!can_buy_gun(gun_type, inventory))
        return;
    inventory.set_gun(std::make_unique<Gun>(gun_type, gun_config));
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

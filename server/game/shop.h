#pragma once

#include <map>
#include <memory>

#include "common/models.h"
#include "server/items/gun.h"
#include "server/player/inventory.h"

#define PRICE_AK47 2700
#define PRICE_M3 1700
#define PRICE_AWP 4750

#define PRICE_MAG_AK47 80
#define PRICE_MAG_M3 8
#define PRICE_MAG_AWP 125
#define PRICE_MAG_GLOCK 20

class Shop {
private:
    std::map<GunType, int> gun_prices;
    std::map<GunType, int> ammo_prices;

public:
    Shop() {
        gun_prices[GunType::AK47] = PRICE_AK47;
        gun_prices[GunType::M3] = PRICE_M3;
        gun_prices[GunType::AWP] = PRICE_AWP;

        ammo_prices[GunType::AK47] = PRICE_MAG_AK47;
        ammo_prices[GunType::M3] = PRICE_MAG_M3;
        ammo_prices[GunType::AWP] = PRICE_MAG_AWP;
        ammo_prices[GunType::Glock] = PRICE_MAG_GLOCK;
    }

    Shop(const Shop&) = delete;
    Shop& operator=(const Shop&) = delete;

    Shop(Shop&&) = default;
    Shop& operator=(Shop&&) = default;

    std::map<GunType, int> get_gun_prices() const { return gun_prices; }
    std::map<GunType, int> get_ammo_prices() const { return ammo_prices; }

    bool can_buy_gun(const GunType& gun_type, Inventory& inventory) const {
        if (gun_type == GunType::Glock)
            return false;
        return gun_prices.at(gun_type) <= inventory.get_money();
    }

    bool can_buy_ammo(const ItemSlot& slot, Inventory& inventory) const {
        if (slot != ItemSlot::Primary && slot != ItemSlot::Secondary)
            return false;

        auto& gun = inventory.get_guns().at(slot);
        GunType gun_type = gun->get_type();
        return ammo_prices.at(gun_type) <= inventory.get_money();
    }

    void buy_gun(const GunType& gun_type, Inventory& inventory) const {
        if (!can_buy_gun(gun_type, inventory))
            return;
        inventory.set_gun(Gun::make_gun(gun_type));
        inventory.set_money(inventory.get_money() - gun_prices.at(gun_type));
    }

    void buy_ammo(const ItemSlot& slot, Inventory& inventory) const {
        if (!can_buy_ammo(slot, inventory))
            return;
        auto& gun = inventory.get_guns().at(slot);
        GunType gun_type = gun->get_type();
        gun->add_mag();
        inventory.set_money(inventory.get_money() - ammo_prices.at(gun_type));
    }

    ~Shop() {}
};

#pragma once

#include <map>
#include <memory>

#include "common/models.h"
#include "server/errors.h"
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

    std::map<GunType, int> get_gun_prices() const { return gun_prices; }
    std::map<GunType, int> get_ammo_prices() const { return ammo_prices; }

    void buy_gun(Inventory& inventory, const GunType& gun_type) const {
        int price = gun_prices.at(gun_type);
        if (inventory.get_money() < price)
            // TODO: Error response
            return;

        inventory.add_primary_weapon(gun_type);
        inventory.set_money(inventory.get_money() - price);
    }

    void buy_ammo(Inventory& inventory, const ItemSlot& slot) const {
        if (slot != ItemSlot::Primary && slot != ItemSlot::Secondary)
            return;
        auto& gun = inventory.get_gun(slot);
        GunType gun_type = gun->get_type();
        int price = ammo_prices.at(gun_type);

        if (inventory.get_money() < price)
            throw BuyAmmoError();

        gun->add_mag();
        inventory.set_money(inventory.get_money() - price);
    }

    ~Shop() {}
};

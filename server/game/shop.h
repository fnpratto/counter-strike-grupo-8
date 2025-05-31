#pragma once

#include <map>

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
    std::map<GunType, int> mag_prices;

public:
    Shop() {
        gun_prices[GunType::AK47] = PRICE_AK47;
        gun_prices[GunType::M3] = PRICE_M3;
        gun_prices[GunType::AWP] = PRICE_AWP;

        mag_prices[GunType::AK47] = PRICE_MAG_AK47;
        mag_prices[GunType::M3] = PRICE_MAG_M3;
        mag_prices[GunType::AWP] = PRICE_MAG_AWP;
        mag_prices[GunType::Glock] = PRICE_MAG_GLOCK;
    }

    std::map<GunType, int> get_gun_prices() const { return gun_prices; }
    std::map<GunType, int> get_mag_prices() const { return mag_prices; }

    // TODO this should take an Inventory
    void buy_gun(Inventory& inventory, const GunType& gun_type) const {
        int price = gun_prices.at(gun_type);
        if (inventory.get_money() < price)
            throw BuyGunError();

        inventory.add_primary_weapon(gun_type);
        inventory.set_money(inventory.get_money() - price);
    }

    // TODO this should take an Inventory
    void buy_ammo(Inventory& inventory, const WeaponSlot& slot) const {
        GunType gun_type = inventory.get_gun(slot)->get_type();
        int price = mag_prices.at(gun_type);

        if (inventory.get_money() < price)
            throw BuyAmmoError();

        inventory.get_gun(slot)->add_mag();
        inventory.set_money(inventory.get_money() - price);
    }

    ~Shop() {}
};

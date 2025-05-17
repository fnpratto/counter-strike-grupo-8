#pragma once

#include <map>

#include "common/models.h"
#include "server/cons.h"

using namespace ShopPrices;

class Shop {
private:
    std::map<GunType, int> gun_prices;
    std::map<GunType, int> mag_prices;

public:
    Shop() {
        gun_prices[GunType::AK47] = price_ak47;
        gun_prices[GunType::M3] = price_m3;
        gun_prices[GunType::AWP] = price_awp;

        mag_prices[GunType::AK47] = price_mag_ak47;
        mag_prices[GunType::M3] = price_mag_m3;
        mag_prices[GunType::AWP] = price_mag_awp;
        mag_prices[GunType::Glock] = price_mag_glock;
    }

    int get_gun_price(const GunType& gun) const { 
        return gun_prices.at(gun); 
    }

    int get_ammo_price(const GunType& gun, int num_mags) const {
        return mag_prices.at(gun) * num_mags;
    }

    ~Shop() {}
};

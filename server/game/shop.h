#pragma once

#include <map>

#include "common/models.h"

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

    int get_gun_price(const GunType& gun) const { return gun_prices.at(gun); }

    int get_ammo_price(const GunType& gun, int num_mags) const {
        return mag_prices.at(gun) * num_mags;
    }

    ~Shop() {}
};

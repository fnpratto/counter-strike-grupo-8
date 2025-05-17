#pragma once

#include <map>

#include "common/models.h"

#define PRICE_AK47 2700
#define PRICE_MAGAZINE_AK47 80

#define PRICE_M3 1700
#define PRICE_MAGAZINE_M3 8

#define PRICE_AWP 4750
#define PRICE_MAGAZINE_AWP 125

#define PRICE_MAGAZINE_GLOCK 20

class Shop {
private:
    std::map<GunType, int> gun_prices;
    std::map<GunType, int> mag_prices;

public:
    Shop() {
        gun_prices[GunType::AK47] = PRICE_AK47;
        gun_prices[GunType::M3] = PRICE_M3;
        gun_prices[GunType::AWP] = PRICE_AWP;

        mag_prices[GunType::AK47] = PRICE_MAGAZINE_AK47;
        mag_prices[GunType::M3] = PRICE_MAGAZINE_M3;
        mag_prices[GunType::AWP] = PRICE_MAGAZINE_AWP;
        mag_prices[GunType::Glock] = PRICE_MAGAZINE_GLOCK;
    }

    int get_gun_price(const GunType& gun) const { 
        return gun_prices.at(gun); 
    }

    /**
     * Return de price of one magazine of the selected gun
     */
    int get_magazine_price(const GunType& gun) const {
        return mag_prices.at(gun);
    }

    ~Shop() {}
};

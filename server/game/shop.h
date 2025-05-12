#pragma once

#include <map>

#include "common/models.h"

#define PRICE_AK47 2700
#define PRICE_M3 1700
#define PRICE_AWP 4750

class Shop {
private:
    std::map<WeaponType, int> weapon_prices;

public:
    Shop() {
        weapon_prices[WeaponType::AK47] = PRICE_AK47;
        weapon_prices[WeaponType::M3] = PRICE_M3;
        weapon_prices[WeaponType::AWP] = PRICE_AWP;
    }

    int get_weapon_price(const WeaponType& weapon) const { 
        return weapon_prices.at(weapon); 
    }

    ~Shop() {}
};

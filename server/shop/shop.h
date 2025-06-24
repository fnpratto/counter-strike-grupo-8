#pragma once

#include <map>

#include "common/models.h"
#include "server/game/game_config.h"
#include "server/player/inventory.h"

class Shop {
private:
    std::map<GunType, int> gun_prices;
    std::map<GunType, int> ammo_prices;

public:
    explicit Shop(const GameConfig::ShopPrices& prices);

    Shop(const Shop&) = delete;
    Shop& operator=(const Shop&) = delete;

    Shop(Shop&&) = default;
    Shop& operator=(Shop&&) = default;

    std::map<GunType, int> get_gun_prices() const;
    std::map<GunType, int> get_ammo_prices() const;

    bool can_buy_gun(const GunType& gun_type, Inventory& inventory) const;

    bool can_buy_ammo(const ItemSlot& slot, Inventory& inventory) const;

    void buy_gun(const GunType& gun_type, Inventory& inventory,
                 const GameConfig::ItemsConfig::GunConfig& gun_config) const;

    void buy_ammo(const ItemSlot& slot, Inventory& inventory) const;
};

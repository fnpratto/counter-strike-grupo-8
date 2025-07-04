#include <gtest/gtest.h>

#include "common/models.h"
#include "server/errors.h"
#include "server/game/game_config.h"
#include "server/items/gun.h"
#include "server/player/player.h"
#include "server/shop/shop.h"

#include "mock_clock.h"

class TestShop: public ::testing::Test {
protected:
    GameConfig config;
    Inventory inventory;

    TestShop():
            config(GameConfig::load_config("./server/config.yaml")),
            inventory(config.player_config.initial_money, config.items_config) {}
};

TEST_F(TestShop, CanBuyAnyPrimaryWeapon) {
    Shop shop(config.shop_prices);
    inventory.set_money(10000);
    int initial_money = inventory.get_full_update().get_money();

    auto prices = shop.get_gun_prices();
    EXPECT_TRUE(prices.find(GunType::AK47) != prices.end());
    EXPECT_TRUE(prices.find(GunType::M3) != prices.end());
    EXPECT_TRUE(prices.find(GunType::AWP) != prices.end());

    for (auto [gun_type, price]: prices) {
        shop.buy_gun(gun_type, inventory, config.items_config.get_gun_config(gun_type));

        int actual_money = inventory.get_updates().get_money();
        EXPECT_EQ(actual_money, initial_money - price);
        initial_money = actual_money;

        InventoryUpdate inv_updates = inventory.get_updates();
        std::map<ItemSlot, GunUpdate> guns_updates = inv_updates.get_guns();
        EXPECT_EQ(guns_updates.at(ItemSlot::Primary).get_gun(), gun_type);
    }
}

TEST_F(TestShop, CannotBuyWeaponIfNotEnoughMoney) {
    Shop shop(config.shop_prices);
    GunType gun = GunType::AK47;
    int initial_money = inventory.get_full_update().get_money();
    int gun_price = shop.get_gun_prices().at(gun);

    while (gun_price <= initial_money) {
        EXPECT_TRUE(shop.can_buy_gun(gun, inventory));
        shop.buy_gun(gun, inventory, config.items_config.get_gun_config(gun));
        initial_money = inventory.get_updates().get_money();
    }

    EXPECT_FALSE(shop.can_buy_gun(gun, inventory));
    EXPECT_EQ(inventory.get_full_update().get_money(), initial_money);
}

TEST_F(TestShop, BuyAmmo) {
    InventoryUpdate inventory_update;

    Shop shop(config.shop_prices);
    inventory.set_money(10000);

    inventory_update = inventory.get_full_update();
    int old_money = inventory_update.get_money();
    GunUpdate old_glock = inventory_update.get_guns().at(ItemSlot::Secondary);

    shop.buy_ammo(ItemSlot::Secondary, inventory);
    InventoryUpdate new_update = inventory.get_full_update();
    int new_money = new_update.get_money();

    EXPECT_EQ(new_money, old_money - shop.get_ammo_prices().at(GunType::Glock));

    GunUpdate new_glock = new_update.get_guns().at(ItemSlot::Secondary);
    EXPECT_EQ(new_glock.get_mag_ammo(), old_glock.get_mag_ammo());
    EXPECT_EQ(new_glock.get_reserve_ammo(),
              old_glock.get_reserve_ammo() + config.items_config.glock.bullets_per_mag);
}

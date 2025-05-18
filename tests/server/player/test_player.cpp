#include <gtest/gtest.h>

#include "server/player/player.h"
#include "common/models.h"
#include "server/errors.h"
#include "server/clock/mock_clock.h"
#include "server/cons.h"
#include "server/weapons/gun.h"
#include "server/game/shop.h"

using namespace PlayerInitialConfig;

class TestPlayer : public ::testing::Test {
protected:
    Player player;

    TestPlayer() : 
            player(Team::Terrorist, Inventory(), full_health, Vector2D()) {}
};

TEST_F(TestPlayer, PlayerStartWithFullHealth) {
    EXPECT_EQ(full_health, player.get_health());
}

TEST_F(TestPlayer, PlayerStartWithDefaultInventory) {
    Inventory i_inv;
    Inventory p_inv = player.get_inventory();
    
    EXPECT_EQ(i_inv.get_money(), p_inv.get_money());
    EXPECT_EQ(i_inv.get_sec_weapon()->get_type(), 
                    p_inv.get_sec_weapon()->get_type());
    EXPECT_EQ(i_inv.get_sec_weapon()->get_mag_ammo(), 
                    p_inv.get_sec_weapon()->get_mag_ammo());
    EXPECT_EQ(i_inv.get_sec_weapon()->get_reserve_ammo(), 
                    p_inv.get_sec_weapon()->get_reserve_ammo());
    EXPECT_EQ(i_inv.get_melee_weapon()->get_type(),
                    p_inv.get_melee_weapon()->get_type());
    EXPECT_THROW({
        p_inv.get_prim_weapon();
    }, std::out_of_range);
    EXPECT_THROW({
        p_inv.get_bomb();
    }, std::out_of_range);
}

TEST_F(TestPlayer, CanBuyAnyPrimaryWeapon) {
    Shop shop;
    player.gain_money(10000);
    
    std::vector<GunType> guns = {GunType::AK47, GunType::M3, GunType::AWP};
    for (GunType g : guns) {
        int initial_money = player.get_inventory().get_money();
        int gun_price = shop.get_gun_price(g);
        player.buy_gun(g, gun_price);
        EXPECT_EQ(player.get_inventory().get_money(), initial_money - gun_price);
        EXPECT_EQ(player.get_inventory().get_prim_weapon()->get_type(), g);
    }
}

TEST_F(TestPlayer, CannotBuyWeaponIfNotEnoughMoney) {
    Shop shop;
    GunType gun = GunType::AK47;
    int initial_money = player.get_inventory().get_money();
    int gun_price = shop.get_gun_price(gun);
    
    while (gun_price <= initial_money) {
        player.buy_gun(gun, gun_price);
        initial_money = player.get_inventory().get_money();
    }

    EXPECT_THROW({
        player.buy_gun(gun, gun_price);
    }, BuyGunError);
    
    EXPECT_EQ(player.get_inventory().get_money(), initial_money);
    
    EXPECT_THROW({
        player.get_inventory().get_prim_weapon();
    }, std::out_of_range);
}

TEST_F(TestPlayer, BuyAmmo) {
    Shop shop;
    player.gain_money(10000);

    Inventory old_inv = player.get_inventory();
    player.buy_ammo(WeaponSlot::Secondary, shop.get_ammo_price(GunType::Glock, 1), 1);
    Inventory new_inv = player.get_inventory();

    EXPECT_EQ(new_inv.get_money(), old_inv.get_money() - ShopPrices::price_mag_glock);
    
    std::unique_ptr<Gun> old_glock = old_inv.get_sec_weapon();
    std::unique_ptr<Gun> new_glock = new_inv.get_sec_weapon();
    EXPECT_EQ(new_glock->get_mag_ammo(), old_glock->get_mag_ammo());
    EXPECT_EQ(new_glock->get_reserve_ammo(), 
                    old_glock->get_reserve_ammo() + 
                            new_glock->get_bullets_per_mag());
}

#include <gtest/gtest.h>

#include "common/models.h"
#include "server/clock/mock_clock.h"
#include "server/errors.h"
#include "server/game/shop.h"
#include "server/player/player.h"
#include "server/player/player_config.h"
#include "server/weapons/gun.h"

class TestPlayer: public ::testing::Test {
protected:
    Player player;

    TestPlayer(): player(Team::TT, Vector2D(0, 0)) {}
};

TEST_F(TestPlayer, PlayerStartWithFullHealth) {
    EXPECT_EQ(PlayerConfig::full_health, player.get_state().health);
}

TEST_F(TestPlayer, PlayerStartWithInitialMoney) {
    EXPECT_EQ(PlayerConfig::initial_money, player.get_state().money);
}

TEST_F(TestPlayer, PlayerStartWithDefaultInventory) {
    InventoryState i_inv = Inventory().full_state();
    InventoryState p_inv = player.get_state().inventory;

    GunState i_sec_weapon = i_inv.guns.at(WeaponSlot::Secondary);
    GunState p_sec_weapon = p_inv.guns.at(WeaponSlot::Secondary);
    EXPECT_EQ(i_sec_weapon.gun, p_sec_weapon.gun);
    EXPECT_EQ(i_sec_weapon.bullets_per_mag, p_sec_weapon.bullets_per_mag);
    EXPECT_EQ(i_sec_weapon.mag_ammo, p_sec_weapon.mag_ammo);
    EXPECT_EQ(i_sec_weapon.reserve_ammo, p_sec_weapon.reserve_ammo);

    UtilityState i_melee = i_inv.utilities.at(WeaponSlot::Melee);
    UtilityState p_melee = p_inv.utilities.at(WeaponSlot::Melee);
    EXPECT_EQ(i_melee.type, p_melee.type);

    EXPECT_THROW({ p_inv.guns.at(WeaponSlot::Primary); }, std::out_of_range);
    EXPECT_THROW({ p_inv.guns.at(WeaponSlot::Bomb); }, std::out_of_range);
}

TEST_F(TestPlayer, CanBuyAnyPrimaryWeapon) {
    Shop shop;
    player.gain_money(10000);
    int initial_money = player.get_state().money;

    std::vector<GunType> guns = {GunType::AK47, GunType::M3, GunType::AWP};
    for (GunType g: guns) {
        player.clear_updates();
        int gun_price = shop.get_gun_price(g);
        player.buy_gun(g, gun_price);

        int actual_money = player.get_updates().get_money();
        EXPECT_EQ(actual_money, initial_money - gun_price);
        initial_money = actual_money;

        InventoryUpdate inv_updates = player.get_updates().get_inventory();
        std::map<WeaponSlot, GunUpdate> guns_updates = inv_updates.get_guns();
        EXPECT_EQ(guns_updates.at(WeaponSlot::Primary).get_gun(), g);
    }
}

TEST_F(TestPlayer, CannotBuyWeaponIfNotEnoughMoney) {
    Shop shop;
    GunType gun = GunType::AK47;
    int initial_money = player.get_state().money;
    int gun_price = shop.get_gun_price(gun);

    while (gun_price <= initial_money) {
        player.buy_gun(gun, gun_price);
        initial_money = player.get_updates().get_money();
    }

    EXPECT_THROW({ player.buy_gun(gun, gun_price); }, BuyGunError);
    EXPECT_EQ(player.get_state().money, initial_money);
}

TEST_F(TestPlayer, BuyAmmo) {
    Shop shop;
    player.gain_money(10000);

    PlayerState old_state = player.get_state();
    int ammo_price = shop.get_ammo_price(GunType::Glock, 1);
    player.buy_ammo(WeaponSlot::Secondary, ammo_price, 1);
    PlayerState new_state = player.get_state();

    EXPECT_EQ(new_state.money, old_state.money - ammo_price);

    GunState old_glock = old_state.inventory.guns.at(WeaponSlot::Secondary);
    GunState new_glock = new_state.inventory.guns.at(WeaponSlot::Secondary);
    EXPECT_EQ(new_glock.mag_ammo, old_glock.mag_ammo);
    EXPECT_EQ(new_glock.reserve_ammo, old_glock.reserve_ammo + new_glock.bullets_per_mag);
}

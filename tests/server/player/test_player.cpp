#include <algorithm>
#include <vector>

#include <gtest/gtest.h>

#include "common/models.h"
#include "server/clock/mock_clock.h"
#include "server/errors.h"
#include "server/game/shop.h"
#include "server/player/player.h"
#include "server/player/player_config.h"
#include "server/weapons/gun.h"

// Include shop price constants
#define PRICE_AK47 2700
#define PRICE_M3 1700
#define PRICE_AWP 4750
#define PRICE_MAG_GLOCK 20

class TestPlayer: public ::testing::Test {
protected:
    MockClock clock;
    Player player;

    TestPlayer(): clock(std::chrono::steady_clock::now()), player(Team::TT, Vector2D(0, 0)) {}

    void advance_secs(float secs) { clock.advance(std::chrono::duration<float>(secs)); }
};

TEST_F(TestPlayer, PlayerStartWithFullHealth) {
    EXPECT_EQ(PlayerConfig::full_health, player.get_full_update().get_health());
}

TEST_F(TestPlayer, PlayerStartWithDefaultInventory) {
    InventoryUpdate i_inv = Inventory().get_full_update();
    InventoryUpdate p_inv = player.get_full_update().get_inventory();

    GunUpdate i_sec_weapon = i_inv.get_guns().at(ItemSlot::Secondary);
    GunUpdate p_sec_weapon = p_inv.get_guns().at(ItemSlot::Secondary);
    EXPECT_EQ(i_sec_weapon.get_gun(), p_sec_weapon.get_gun());
    EXPECT_EQ(i_sec_weapon.get_mag_ammo(), p_sec_weapon.get_mag_ammo());
    EXPECT_EQ(i_sec_weapon.get_reserve_ammo(), p_sec_weapon.get_reserve_ammo());

    EXPECT_TRUE(p_inv.get_guns().find(ItemSlot::Secondary) != p_inv.get_guns().end());
    EXPECT_FALSE(p_inv.get_guns().find(ItemSlot::Primary) != p_inv.get_guns().end());
    EXPECT_TRUE(p_inv.get_knife().has_change());
    EXPECT_FALSE(p_inv.get_bomb().has_value());
}

TEST_F(TestPlayer, GunReduceMagAmmoWhenPlayerAttacks) {
    player.equip_item(ItemSlot::Secondary);
    int old_mag_ammo = player.get_inventory().get_gun(ItemSlot::Secondary)->get_mag_ammo();

    player.start_attacking();
    auto attack_effects = player.attack(clock.now());
    EXPECT_EQ(attack_effects.size(), 1);
    EXPECT_EQ(player.get_inventory().get_gun(ItemSlot::Secondary)->get_mag_ammo(),
              old_mag_ammo - GlockConfig.bullets_per_attack);
}

TEST_F(TestPlayer, GunBurst) {
    Shop shop;
    player.get_inventory().set_money(10000);
    shop.buy_gun(GunType::AK47, player.get_inventory());

    player.equip_item(ItemSlot::Primary);
    player.start_attacking();

    for (int i = 0; i < Ak47Config.bullets_per_attack; i++) {
        if (i > 0) {
            auto effects = player.attack(clock.now());
            EXPECT_EQ(effects.size(), 0);
        }
        advance_secs(Ak47Config.burst_interval);
        auto attack_effects = player.attack(clock.now());
        EXPECT_EQ(attack_effects.size(), 1);
        EXPECT_EQ(player.get_inventory().get_gun(ItemSlot::Primary)->get_mag_ammo(),
                  Ak47Config.bullets_per_mag - (i + 1));
    }

    auto attack_effects = player.attack(clock.now());
    EXPECT_EQ(attack_effects.size(), 0);
}

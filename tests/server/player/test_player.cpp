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
    Player player;

    TestPlayer(): player(Team::TT, Vector2D(0, 0)) {}
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

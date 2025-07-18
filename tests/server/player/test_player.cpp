#include <algorithm>
#include <vector>

#include <gtest/gtest.h>

#include "../game/mock_clock.h"
#include "common/models.h"
#include "server/errors.h"
#include "server/game/game_config.h"
#include "server/items/gun.h"
#include "server/physics/circular_hitbox.h"
#include "server/player/player.h"
#include "server/shop/shop.h"

class TestPlayer: public ::testing::Test {
protected:
    GameConfig config;
    MockClock clock;
    Player player;

    TestPlayer():
            config(GameConfig::load_config("./server/config.yaml")),
            clock(std::chrono::steady_clock::now()),
            player(Team::TT, CharacterType::Pheonix, CircularHitbox::player_hitbox(Vector2D(0, 0)),
                   config.player_config, config.items_config) {}

    void advance_secs(float secs) { clock.advance(std::chrono::duration<float>(secs)); }
};

TEST_F(TestPlayer, PlayerStartWithFullHealth) {
    EXPECT_EQ(config.player_config.full_health, player.get_full_update().get_health());
}

TEST_F(TestPlayer, PlayerStartWithDefaultInventory) {
    InventoryUpdate i_inv =
            Inventory(config.player_config.initial_money, config.items_config).get_full_update();
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
    int old_mag_ammo = player.get_inventory().get_guns().at(ItemSlot::Secondary)->get_mag_ammo();

    player.handle_start_attacking(clock.now());
    auto attack_effects = player.attack(clock.now());
    EXPECT_EQ(attack_effects.size(), 1);
    EXPECT_EQ(player.get_inventory().get_guns().at(ItemSlot::Secondary)->get_mag_ammo(),
              old_mag_ammo - config.items_config.glock.bullets_per_attack);
}

TEST_F(TestPlayer, GunBurst) {
    Shop shop(config.shop_prices);
    player.get_inventory().set_money(10000);
    shop.buy_gun(GunType::AK47, player.get_inventory(),
                 config.items_config.get_gun_config(GunType::AK47));

    player.equip_item(ItemSlot::Primary);
    player.handle_start_attacking(clock.now());

    for (int i = 0; i < config.items_config.ak47.bullets_per_attack; i++) {
        if (i > 0) {
            auto effects = player.attack(clock.now());
            EXPECT_EQ(effects.size(), 0);
        }
        advance_secs(config.items_config.ak47.burst_interval);
        auto attack_effects = player.attack(clock.now());
        EXPECT_EQ(attack_effects.size(), 1);
        EXPECT_EQ(player.get_inventory().get_guns().at(ItemSlot::Primary)->get_mag_ammo(),
                  config.items_config.ak47.bullets_per_mag - (i + 1));
    }

    auto attack_effects = player.attack(clock.now());
    EXPECT_EQ(attack_effects.size(), 0);
}

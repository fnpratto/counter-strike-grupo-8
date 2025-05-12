#include <gtest/gtest.h>
#include <vector>

#include "server/game/player.h"
#include "common/models.h"
#include "server/errors.h"
#include "server/game/game.h"
#include "server/game/game_config.h"
#include "server/clock/mock_clock.h"

class TestPlayer : public ::testing::Test {
protected:
    std::string game_name;
    std::string player_name;

    TestPlayer() : 
            game_name("test_game"),
            player_name("test_player") {}
};

TEST_F(TestPlayer, CanChangeTeam) {
    MockClock clock(std::chrono::steady_clock::now());
    Game game(game_name, clock, GameConfig(), Shop());
    game.join(player_name);

    EXPECT_EQ(game.get_player_team(player_name), Team::Terrorist);
    game.change_player_team(player_name, Team::CounterTerrorist);
    EXPECT_EQ(game.get_player_team(player_name), Team::CounterTerrorist);
}

TEST_F(TestPlayer, DefaultInventory) {
    MockClock clock(std::chrono::steady_clock::now());
    GameConfig config;
    Game game(game_name, clock, config, Shop());
    game.join(player_name);
    
    Inventory default_inventory = config.get_initial_inventory();
    Inventory player_inventory = game.get_player_inventory(player_name);
    
    EXPECT_EQ(default_inventory.money, player_inventory.money);
    EXPECT_EQ(default_inventory.weapons.size(), player_inventory.weapons.size());
    EXPECT_EQ(default_inventory.weapons.size(), 3);
    EXPECT_EQ(default_inventory.weapons.at(WeaponSlot::Secondary), WeaponType::Glock);
    EXPECT_EQ(default_inventory.weapons.at(WeaponSlot::Melee), WeaponType::Knife);
    EXPECT_EQ(default_inventory.weapons.at(WeaponSlot::Bomb), WeaponType::C4);
}

TEST_F(TestPlayer, CanBuyAnyPrimaryWeapon) {
    MockClock clock(std::chrono::steady_clock::now());
    GameConfig config;

    Inventory inventory = config.get_initial_inventory();
    inventory.money = 10000;
    config.set_initial_inventory(inventory);

    Shop shop;
    Game game(game_name, clock, config, shop);
    game.join(player_name);
    
    std::vector<WeaponType> weapons = {WeaponType::AK47, WeaponType::M3, WeaponType::AWP};
    for (WeaponType w : weapons) {
        int initial_money = game.get_player_inventory(player_name).money;
        int weapon_price = shop.get_weapon_price(w);
        game.player_buy_weapon(player_name, w);
        EXPECT_EQ(game.get_player_inventory(player_name).money, initial_money - weapon_price);
        EXPECT_EQ(game.get_player_inventory(player_name).weapons.at(WeaponSlot::Primary), w);
    }
}

TEST_F(TestPlayer, CannotBuyWeaponIfNotEnoughMoney) {
    MockClock clock(std::chrono::steady_clock::now());
    Shop shop;
    Game game(game_name, clock, GameConfig(), shop);
    game.join(player_name);

    WeaponType weapon = WeaponType::AK47;
    int initial_money = game.get_player_inventory(player_name).money;
    int exp_weapon_price = shop.get_weapon_price(weapon);
    
    while (exp_weapon_price <= initial_money) {
        game.player_buy_weapon(player_name, weapon);
        initial_money = game.get_player_inventory(player_name).money;
    }

    EXPECT_THROW({
        game.player_buy_weapon(player_name, weapon);
    }, BuyWeaponError);
    
    EXPECT_EQ(game.get_player_inventory(player_name).money, initial_money);
    
    EXPECT_THROW({
        game.get_player_inventory(player_name).weapons.at(WeaponSlot::Primary);
    }, std::out_of_range);
}

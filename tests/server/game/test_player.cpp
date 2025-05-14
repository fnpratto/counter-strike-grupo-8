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

TEST_F(TestPlayer, DefaultInventory) {
    MockClock clock(std::chrono::steady_clock::now());
    Game game(game_name, clock, GameConfig(), Shop());
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, player_name);
    
    Inventory default_inventory = game.get_config().get_default_inventory();
    Inventory player_inventory = game.get_config().get_player_inventory(player_name);
    
    EXPECT_EQ(default_inventory.money, player_inventory.money);
    EXPECT_EQ(default_inventory.weapons.at(WeaponSlot::Secondary), WeaponType::Glock);
    EXPECT_EQ(default_inventory.weapons.at(WeaponSlot::Melee), WeaponType::Knife);
    EXPECT_THROW({
        default_inventory.weapons.at(WeaponSlot::Primary);
    }, std::out_of_range);
}

TEST_F(TestPlayer, OneTerroristHasBombWhenGameStarted) {
    MockClock clock(std::chrono::steady_clock::now());
    Game game(game_name, clock, GameConfig(), Shop());
    
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, player_name);
    Message msg_select_team = Message(SelectTeamCommand(Team::Terrorist));
    game.tick(msg_select_team, player_name);

    std::string another_player_name = "another_player";
    game.tick(msg_join, another_player_name);
    game.tick(msg_select_team, another_player_name);

    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "test_player");

    Inventory first_tt_invent = game.get_config().get_player_inventory(player_name);
    Inventory second_tt_invent = game.get_config().get_player_inventory(another_player_name);
    if (first_tt_invent.weapons.find(WeaponSlot::Bomb) != first_tt_invent.weapons.end()) {
        EXPECT_TRUE(second_tt_invent.weapons.find(WeaponSlot::Bomb) == second_tt_invent.weapons.end());
    } else if (second_tt_invent.weapons.find(WeaponSlot::Bomb) != second_tt_invent.weapons.end()) {
        EXPECT_TRUE(first_tt_invent.weapons.find(WeaponSlot::Bomb) == first_tt_invent.weapons.end());
    } else {
        FAIL();
    }
}

TEST_F(TestPlayer, CounterTerroristDoesNotHaveBombWhenGameStarted) {
    MockClock clock(std::chrono::steady_clock::now());
    GameConfig config;
    Game game(game_name, clock, config, Shop());

    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, player_name);
    Message msg_select_team = Message(SelectTeamCommand(Team::CounterTerrorist));
    game.tick(msg_select_team, player_name);

    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "test_player");

    Inventory ct_invent = game.get_config().get_player_inventory(player_name);
    EXPECT_TRUE(ct_invent.weapons.find(WeaponSlot::Bomb) == ct_invent.weapons.end());
}

TEST_F(TestPlayer, CanBuyAnyPrimaryWeapon) {
    MockClock clock(std::chrono::steady_clock::now());
    GameConfig conf;
    Inventory inventory = conf.get_default_inventory();
    inventory.money = 10000;
    conf.set_default_inventory(inventory);

    Shop shop;
    Game game(game_name, clock, conf, shop);
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, player_name);
    
    std::vector<WeaponType> weapons = {WeaponType::AK47, WeaponType::M3, WeaponType::AWP};
    for (WeaponType w : weapons) {
        int initial_money = game.get_config().get_player_inventory(player_name).money;
        int weapon_price = shop.get_weapon_price(w);

        Message msg_buy = Message(BuyWeaponCommand(w));
        game.tick(msg_buy, player_name);
        
        EXPECT_EQ(game.get_config().get_player_inventory(player_name).money, initial_money - weapon_price);
        EXPECT_EQ(game.get_config().get_player_inventory(player_name).weapons.at(WeaponSlot::Primary), w);
    }
}

TEST_F(TestPlayer, CannotBuyWeaponIfNotEnoughMoney) {
    MockClock clock(std::chrono::steady_clock::now());
    Shop shop;
    Game game(game_name, clock, GameConfig(), shop);
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, player_name);

    WeaponType weapon = WeaponType::AK47;
    int initial_money = game.get_config().get_player_inventory(player_name).money;
    int exp_weapon_price = shop.get_weapon_price(weapon);
    
    Message msg_buy = Message(BuyWeaponCommand(weapon));
    while (exp_weapon_price <= initial_money) {
        game.tick(msg_buy, player_name);
        initial_money = game.get_config().get_player_inventory(player_name).money;
    }

    EXPECT_THROW({
        game.tick(msg_buy, player_name);
    }, BuyWeaponError);
    
    EXPECT_EQ(game.get_config().get_player_inventory(player_name).money, initial_money);
    
    EXPECT_THROW({
        game.get_config().get_player_inventory(player_name).weapons.at(WeaponSlot::Primary);
    }, std::out_of_range);
}

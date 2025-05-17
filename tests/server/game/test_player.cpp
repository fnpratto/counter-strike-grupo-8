#include <gtest/gtest.h>
#include <vector>

#include "server/player/player.h"
#include "common/models.h"
#include "server/errors.h"
#include "server/clock/mock_clock.h"
#include "server/game/game.h"
#include "server/game/game_config.h"
#include "server/weapons/gun.h"

class TestPlayer : public ::testing::Test {
protected:
    std::string game_name;
    std::string player_name;

    TestPlayer() : 
            game_name("test_game"),
            player_name("test_player") {}
};

TEST_F(TestPlayer, PlayerStartWithFullHealth) {
    MockClock clock(std::chrono::steady_clock::now());
    Game game(game_name, clock, GameConfig(), Shop());
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, player_name);
    
    unsigned int full_health = game.get_config().get_full_health();
    unsigned int player_health = game.get_config().get_player(player_name).get_health();
    
    EXPECT_EQ(full_health, player_health);
}

TEST_F(TestPlayer, PlayerStartWithDefaultInventory) {
    MockClock clock(std::chrono::steady_clock::now());
    GameConfig conf;
    Game game(game_name, clock, conf, Shop());
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, player_name);
    
    Inventory d_inv = game.get_config().get_default_inventory();
    Inventory p_inv = game.get_config().get_player(player_name).get_inventory();
    
    EXPECT_EQ(d_inv.get_money(), p_inv.get_money());
    EXPECT_EQ(d_inv.get_sec_weapon()->get_type(), 
                    p_inv.get_sec_weapon()->get_type());
    EXPECT_EQ(d_inv.get_sec_weapon()->get_magazine_ammo(), 
                    p_inv.get_sec_weapon()->get_magazine_ammo());
    EXPECT_EQ(d_inv.get_sec_weapon()->get_reserve_ammo(), 
                    p_inv.get_sec_weapon()->get_reserve_ammo());
    EXPECT_EQ(d_inv.get_melee_weapon()->get_type(),
                    p_inv.get_melee_weapon()->get_type());
    EXPECT_THROW({
        p_inv.get_prim_weapon();
    }, std::out_of_range);
    EXPECT_THROW({
        p_inv.get_bomb();
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

    Player tt = game.get_config().get_player(player_name);
    Player another_tt = game.get_config().get_player(another_player_name);

    if (tt.get_inventory().has_bomb()) {
        EXPECT_FALSE(another_tt.get_inventory().has_bomb());
    } else if (another_tt.get_inventory().has_bomb()) {
        EXPECT_FALSE(tt.get_inventory().has_bomb());
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

    Inventory ct_invent = game.get_config().get_player(player_name).get_inventory();
    EXPECT_FALSE(ct_invent.has_bomb());
}

TEST_F(TestPlayer, CanBuyAnyPrimaryWeapon) {
    MockClock clock(std::chrono::steady_clock::now());
    GameConfig conf;
    Inventory inventory = conf.get_default_inventory();
    inventory.add_money(10000);
    conf.set_default_inventory(inventory);

    Shop shop;
    Game game(game_name, clock, conf, shop);
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, player_name);
    
    std::vector<GunType> guns = {GunType::AK47, GunType::M3, GunType::AWP};
    for (GunType g : guns) {
        int initial_money = game.get_config().get_player(player_name).get_inventory().get_money();
        int gun_price = shop.get_gun_price(g);

        Message msg_buy = Message(BuyGunCommand(g));
        game.tick(msg_buy, player_name);
        
        Player player = game.get_config().get_player(player_name);
        EXPECT_EQ(player.get_inventory().get_money(), initial_money - gun_price);
        EXPECT_EQ(player.get_inventory().get_prim_weapon()->get_type(), g);
    }
}

TEST_F(TestPlayer, CannotBuyWeaponIfNotEnoughMoney) {
    MockClock clock(std::chrono::steady_clock::now());
    Shop shop;
    Game game(game_name, clock, GameConfig(), shop);
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, player_name);

    GunType gun = GunType::AK47;
    int initial_money = game.get_config().get_player(player_name).get_inventory().get_money();
    int exp_gun_price = shop.get_gun_price(gun);
    
    Message msg_buy = Message(BuyGunCommand(gun));
    while (exp_gun_price <= initial_money) {
        game.tick(msg_buy, player_name);
        initial_money = game.get_config().get_player(player_name).get_inventory().get_money();
    }

    EXPECT_THROW({
        game.tick(msg_buy, player_name);
    }, BuyGunError);
    
    EXPECT_EQ(game.get_config().get_player(player_name).get_inventory().get_money(), initial_money);
    
    EXPECT_THROW({
        game.get_config().get_player(player_name).get_inventory().get_prim_weapon();
    }, std::out_of_range);
}

TEST_F(TestPlayer, CanBuyAmmo) {
    MockClock clock(std::chrono::steady_clock::now());
    GameConfig conf;
    Inventory inventory = conf.get_default_inventory();
    inventory.add_money(10000);
    conf.set_default_inventory(inventory);

    Shop shop;
    Game game(game_name, clock, conf, shop);
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, player_name);
    
    Inventory old_inven = game.get_config().get_player(player_name).get_inventory();
    Message msg_buy_ammo = Message(BuyAmmoCommand(GunType::Glock));
    game.tick(msg_buy_ammo, player_name);
    Inventory new_inven = game.get_config().get_player(player_name).get_inventory();

    EXPECT_EQ(new_inven.get_money(), old_inven.get_money() - shop.get_magazine_price(GunType::Glock));
    
    std::unique_ptr<Gun> old_glock = old_inven.get_sec_weapon();
    std::unique_ptr<Gun> new_glock = new_inven.get_sec_weapon();
    EXPECT_EQ(new_glock->get_magazine_ammo(), old_glock->get_magazine_ammo());
    EXPECT_EQ(new_glock->get_reserve_ammo(), 
                    old_glock->get_reserve_ammo() + 
                            new_glock->get_bullets_per_magazine());
}

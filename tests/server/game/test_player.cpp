#include <gtest/gtest.h>
#include <vector>

#include "server/game/player.h"
#include "server/game/shop.h"
#include "common/models.h"
#include "server/errors.h"

TEST(TestPlayer, CanChangeTeam) {
    Player player;
    EXPECT_EQ(player.get_team(), Team::Terrorist);
    player.change_team(Team::CounterTerrorist);
    EXPECT_EQ(player.get_team(), Team::CounterTerrorist);
}

TEST(TestPlayer, CanBuyAnyPrimaryWeapon) {
    Player player;
    Shop shop;
    
    std::vector<WeaponType> weapons = {WeaponType::AK47, WeaponType::M3, WeaponType::AWP};
    for (WeaponType w : weapons) {
        int weapon_price = shop.get_weapon_price(w);
        player.gain_money(weapon_price);
        int initial_money = player.get_inventory().money;
        player.buy_weapon(w, weapon_price);
        EXPECT_EQ(player.get_inventory().money, initial_money - weapon_price);
        EXPECT_EQ(player.get_inventory().weapons[WeaponSlot::Primary], w);
    }
}

TEST(TestPlayer, CannotBuyWeaponIfNotEnoughMoney) {
    Player player;
    Shop shop;
    

    WeaponType weapon = WeaponType::AK47;
    int initial_money = player.get_inventory().money;
    int exp_weapon_price = shop.get_weapon_price(weapon);
    
    while (exp_weapon_price <= initial_money) {
        player.buy_weapon(weapon, exp_weapon_price);
        initial_money = player.get_inventory().money;
    }

    EXPECT_THROW({
        player.buy_weapon(weapon, exp_weapon_price);
    }, BuyWeaponError);
    
    EXPECT_EQ(player.get_inventory().money, initial_money);
    
    EXPECT_THROW({
        player.get_inventory().weapons.at(WeaponSlot::Primary);
    }, std::out_of_range);
}


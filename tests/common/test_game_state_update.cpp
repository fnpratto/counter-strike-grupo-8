#include <gtest/gtest.h>

#include "common/updates/game_update.h"
#include "common/updates/gun_update.h"
#include "common/updates/player_update.h"

class TestGameUpdate: public ::testing::Test {
protected:
    GameUpdate game_update;
};

TEST_F(TestGameUpdate, InitialState) { EXPECT_FALSE(game_update.has_change()); }

TEST_F(TestGameUpdate, ClearUpdate) {
    game_update.set_num_rounds(1);
    EXPECT_TRUE(game_update.has_change());
    game_update.clear();
    EXPECT_FALSE(game_update.has_change());
}

TEST_F(TestGameUpdate, OptionalMerging) {
    GameUpdate update1;
    GameUpdate update2;

    // Test merging when first has value, second doesn't
    update1.set_num_rounds(5);
    auto merged = update1.merged(update2);
    EXPECT_TRUE(merged.has_num_rounds_changed());
    EXPECT_EQ(merged.get_num_rounds(), 5);

    // Test merging when second has value, first doesn't
    GameUpdate update3;
    GameUpdate update4;
    update4.set_num_rounds(2);
    merged = update3.merged(update4);
    EXPECT_TRUE(merged.has_num_rounds_changed());
    EXPECT_EQ(merged.get_num_rounds(), 2);

    // Test merging when both have values (second should win)
    GameUpdate update5;
    GameUpdate update6;
    update5.set_num_rounds(2);
    update6.set_num_rounds(3);
    merged = update5.merged(update6);
    EXPECT_TRUE(merged.has_num_rounds_changed());
    EXPECT_EQ(merged.get_num_rounds(), 3);
}

TEST_F(TestGameUpdate, MapMerging) {
    GameUpdate update1;
    GameUpdate update2;

    // Set players in initial update
    PlayerUpdate initial_player1;
    initial_player1.set_health(100);

    PlayerUpdate initial_player2;
    initial_player2.set_health(75);
    initial_player2.set_team(Team::CT);

    std::map<std::string, PlayerUpdate> initial_players;
    initial_players["player1"] = initial_player1;
    initial_players["player2"] = initial_player2;
    update1.set_players(initial_players);

    // Set players in second update (overlapping and new players)
    PlayerUpdate player2_update;
    player2_update.set_ready(true);

    PlayerUpdate new_player;
    new_player.set_pos(Vector2D(10, 0));

    std::map<std::string, PlayerUpdate> players_update;
    players_update["player2"] = player2_update;  // This should merge with existing player2
    players_update["player4"] = new_player;      // New player
    update2.set_players(players_update);

    // Merge the updates
    auto merged = update1.merged(update2);
    EXPECT_TRUE(merged.has_players_changed());

    auto merged_players = merged.get_players();

    // Check that we have 3 players
    EXPECT_EQ(merged_players.size(), 3);

    // Check player1 remains unchanged
    EXPECT_TRUE(merged_players.count("player1"));
    EXPECT_TRUE(merged_players["player1"].has_health_changed());
    EXPECT_EQ(merged_players["player1"].get_health(), 100);

    // Check player2 was merged (should have attributes from both)
    EXPECT_TRUE(merged_players.count("player2"));
    auto& merged_player2 = merged_players["player2"];
    EXPECT_TRUE(merged_player2.has_health_changed());
    EXPECT_EQ(merged_player2.get_health(), 75);  // From original player2
    EXPECT_TRUE(merged_player2.has_team_changed());
    EXPECT_EQ(merged_player2.get_team(), Team::CT);  // From original player2
    EXPECT_TRUE(merged_player2.has_ready_changed());
    EXPECT_EQ(merged_player2.get_ready(), true);  // From player3

    // Check player4 was added
    EXPECT_TRUE(merged_players.count("player4"));
    EXPECT_TRUE(merged_players["player4"].has_pos_changed());
    EXPECT_EQ(merged_players["player4"].get_pos().get_x(), 10);
}

TEST_F(TestGameUpdate, NestedUpdateMerging) {
    GameUpdate update1;
    GameUpdate update2;

    // Create phase updates
    PhaseUpdate phase1;
    phase1.set_type(PhaseType::WarmUp);

    PhaseUpdate phase2;
    phase2.set_secs_remaining(2);  // Assuming TimePoint has default constructor

    update1.set_phase(phase1);
    update2.set_phase(phase2);

    // Merge the updates
    auto merged = update1.merged(update2);
    EXPECT_TRUE(merged.has_phase_changed());

    auto merged_phase = merged.get_phase();
    EXPECT_TRUE(merged_phase.has_type_changed());
    EXPECT_TRUE(merged_phase.has_secs_remaining_changed());
    EXPECT_EQ(merged_phase.get_type(), PhaseType::WarmUp);
}

TEST_F(TestGameUpdate, IndividualChangeDetection) {
    // Test individual change detection methods
    EXPECT_FALSE(game_update.has_phase_changed());
    EXPECT_FALSE(game_update.has_num_rounds_changed());
    EXPECT_FALSE(game_update.has_players_changed());

    // Set some values and check detection
    PhaseUpdate phase;
    phase.set_type(PhaseType::Buying);
    game_update.set_phase(phase);
    EXPECT_TRUE(game_update.has_phase_changed());

    game_update.set_num_rounds(3);
    EXPECT_TRUE(game_update.has_num_rounds_changed());

    std::map<std::string, PlayerUpdate> players;
    PlayerUpdate player;
    player.set_health(50);
    players["test"] = player;
    game_update.set_players(players);
    EXPECT_TRUE(game_update.has_players_changed());
}

TEST_F(TestGameUpdate, SettersAndGetters) {
    // Test that setting values works correctly
    game_update.set_num_rounds(10);
    EXPECT_EQ(game_update.get_num_rounds(), 10);

    // Test phase setting
    PhaseUpdate phase;
    phase.set_type(PhaseType::InRound);
    game_update.set_phase(phase);
    EXPECT_EQ(game_update.get_phase().get_type(), PhaseType::InRound);
}

class TestPlayerUpdate: public ::testing::Test {
protected:
    PlayerUpdate player_update;
};

TEST_F(TestPlayerUpdate, InventoryMerging) {
    PlayerUpdate update1;
    PlayerUpdate update2;

    // Create inventory updates
    InventoryUpdate inv1;
    std::map<ItemSlot, GunUpdate> guns1;
    GunUpdate gun1;
    gun1.set_gun(GunType::AK47);
    gun1.set_mag_ammo(30);
    guns1[ItemSlot::Primary] = gun1;
    inv1.set_guns(guns1);

    InventoryUpdate inv2;
    std::map<ItemSlot, GunUpdate> guns2;
    GunUpdate gun2;
    gun2.set_reserve_ammo(90);  // Same slot, should merge
    guns2[ItemSlot::Primary] = gun2;

    GunUpdate gun3;
    gun3.set_gun(GunType::Glock);
    guns2[ItemSlot::Secondary] = gun3;  // New slot
    inv2.set_guns(guns2);

    update1.set_inventory(inv1);
    update2.set_inventory(inv2);

    auto merged = update1.merged(update2);
    auto merged_inventory = merged.get_inventory();
    auto merged_guns = merged_inventory.get_guns();

    // Should have both weapon slots
    EXPECT_EQ(merged_guns.size(), 2);

    // Primary weapon should have merged attributes
    EXPECT_TRUE(merged_guns.count(ItemSlot::Primary));
    auto& primary = merged_guns[ItemSlot::Primary];
    EXPECT_TRUE(primary.has_gun_changed());
    EXPECT_TRUE(primary.has_mag_ammo_changed());
    EXPECT_TRUE(primary.has_reserve_ammo_changed());
    EXPECT_EQ(primary.get_gun(), GunType::AK47);
    EXPECT_EQ(primary.get_mag_ammo(), 30);
    EXPECT_EQ(primary.get_reserve_ammo(), 90);

    // Secondary weapon should be present
    EXPECT_TRUE(merged_guns.count(ItemSlot::Secondary));
    EXPECT_EQ(merged_guns[ItemSlot::Secondary].get_gun(), GunType::Glock);
}

class TestGunUpdate: public ::testing::Test {
protected:
    GunUpdate gun_update;
};

TEST_F(TestGunUpdate, BasicFunctionality) {
    EXPECT_FALSE(gun_update.has_change());

    gun_update.set_gun(GunType::AK47);
    EXPECT_TRUE(gun_update.has_change());
    EXPECT_TRUE(gun_update.has_gun_changed());
    EXPECT_EQ(gun_update.get_gun(), GunType::AK47);

    gun_update.set_mag_ammo(25);
    EXPECT_TRUE(gun_update.has_mag_ammo_changed());
    EXPECT_EQ(gun_update.get_mag_ammo(), 25);

    gun_update.clear();
    EXPECT_FALSE(gun_update.has_change());
}

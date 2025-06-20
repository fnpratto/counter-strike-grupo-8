#include <chrono>
#include <memory>

#include <gtest/gtest.h>

#include "common/map/map.h"
#include "common/models.h"
#include "common/physics/physics_config.h"
#include "common/updates/game_update.h"
#include "common/updates/inventory_update.h"
#include "common/updates/phase_update.h"
#include "common/updates/player_update.h"
#include "server/clock/mock_clock.h"
#include "server/errors.h"
#include "server/game/game.h"
#include "server/game/game_config.h"
#include "server/game/shop.h"
#include "server/items/items_config.h"
#include "server/map/map_builder.h"
#include "server/player_message.h"

class TestGameBomb: public ::testing::Test {
protected:
    std::shared_ptr<MockClock> clock;
    Map map;
    int max_players;
    Game game;

    TestGameBomb():
            clock(std::make_shared<MockClock>(std::chrono::steady_clock::now())),
            map(MapBuilder("./tests/server/map/map2.yaml").build()),
            max_players(map.get_max_players()),
            game("test_game", clock, std::move(map)) {}

    void SetUp() override {
        game.join_player("tt");
        game.join_player("ct");
        Message msg_select_team_tt = Message(SelectTeamCommand(Team::TT));
        Message msg_select_team_ct = Message(SelectTeamCommand(Team::CT));
        Message msg_set_ready = Message(SetReadyCommand());
        game.tick({PlayerMessage("tt", msg_select_team_tt), PlayerMessage("ct", msg_select_team_ct),
                   PlayerMessage("tt", msg_set_ready), PlayerMessage("ct", msg_set_ready)});
        advance_secs(PhaseTimes::buying_duration);
        game.tick({});
    }

    void SetUpBombPlanted() {
        Message msg_start_planting = Message(StartPlantingBombCommand());
        game.tick({PlayerMessage("tt", msg_start_planting)});
        advance_secs(BombConfig::secs_to_plant);
        game.tick({});
    }

    void advance_secs(float secs) { clock->advance(std::chrono::duration<float>(secs)); }
};

TEST_F(TestGameBomb, PlayerCanStartPlantingBomb) {
    Message msg_start_planting = Message(StartPlantingBombCommand());
    auto player_messages = game.tick({PlayerMessage("tt", msg_start_planting)});
    GameUpdate updates = player_messages[0].get_message().get_content<GameUpdate>();

    EXPECT_TRUE(updates.get_players().at("tt").get_inventory().get_bomb().has_value());
    const BombUpdate& bomb_update =
            updates.get_players().at("tt").get_inventory().get_bomb().value();
    EXPECT_EQ(bomb_update.get_bomb_phase(), BombPhaseType::Planting);

    game.tick({});
    EXPECT_EQ(game.get_full_update().get_phase().get_phase(), PhaseType::Playing);
}

TEST_F(TestGameBomb, PlayerPlantBombAfterPlantingForSecondsToPlantTime) {
    Message msg_start_planting = Message(StartPlantingBombCommand());
    game.tick({PlayerMessage("tt", msg_start_planting)});

    advance_secs(BombConfig::secs_to_plant);
    auto player_messages = game.tick({});

    bool found_bomb_planted_resp = false;
    for (const auto& player_msg: player_messages) {
        if (player_msg.get_message().get_type() ==
            MessageType::BOMB_PLANTED_RESP) {  // cppcheck-suppress[useStlAlgorithm]
            found_bomb_planted_resp = true;
            break;
        }
    }
    EXPECT_TRUE(found_bomb_planted_resp);

    GameUpdate updates = game.get_full_update();
    EXPECT_FALSE(updates.get_players().at("tt").get_inventory().get_bomb().has_value());
    EXPECT_TRUE(updates.get_bomb().has_value());
    EXPECT_EQ(updates.get_bomb().value().item.get_bomb_phase(), BombPhaseType::Planted);
    EXPECT_EQ(updates.get_bomb().value().hitbox.get_pos(),
              updates.get_players().at("tt").get_pos());
}

TEST_F(TestGameBomb, PlayerDoesNotPlantBombIfStopPlantingBeforeSecondsToPlantTime) {
    Message msg_start_planting = Message(StartPlantingBombCommand());
    game.tick({PlayerMessage("tt", msg_start_planting)});

    advance_secs(BombConfig::secs_to_plant / 2.0f);
    Message msg_stop_planting = Message(StopPlantingBombCommand());
    auto player_messages = game.tick({PlayerMessage("tt", msg_stop_planting)});
    GameUpdate updates = player_messages[0].get_message().get_content<GameUpdate>();

    EXPECT_TRUE(updates.get_players().at("tt").get_inventory().get_bomb().has_value());
    const BombUpdate& bomb_update =
            updates.get_players().at("tt").get_inventory().get_bomb().value();
    EXPECT_EQ(bomb_update.get_bomb_phase(), BombPhaseType::NotPlanted);

    game.tick({});
    EXPECT_EQ(game.get_full_update().get_phase().get_phase(), PhaseType::Playing);
}

TEST_F(TestGameBomb, BombExplodeAfterSecondsToExplode) {
    SetUpBombPlanted();
    advance_secs(BombConfig::secs_to_explode);
    auto player_messages = game.tick({});

    bool found_bomb_exploded_resp = false;
    bool found_round_end_resp = false;
    for (const auto& player_msg: player_messages) {
        if (player_msg.get_message().get_type() ==
            MessageType::BOMB_EXPLODED_RESP) {  // cppcheck-suppress[useStlAlgorithm]
            found_bomb_exploded_resp = true;
            auto bomb_exploded_resp = player_msg.get_message().get_content<BombExplodedResponse>();

            GameUpdate updates = game.get_full_update();
            EXPECT_TRUE(bomb_exploded_resp.get_explosion_center() ==
                        updates.get_bomb().value().hitbox.get_center());
            EXPECT_EQ(bomb_exploded_resp.get_explosion_radius(), BombConfig::max_range);
        } else if (player_msg.get_message().get_type() ==
                   MessageType::ROUND_END_RESP) {  // cppcheck-suppress[useStlAlgorithm]
            found_round_end_resp = true;
            auto round_end_resp = player_msg.get_message().get_content<RoundEndResponse>();
            EXPECT_EQ(round_end_resp.get_winning_team(), Team::TT);
        }
    }
    EXPECT_TRUE(found_bomb_exploded_resp);
    EXPECT_TRUE(found_round_end_resp);

    GameUpdate updates = game.get_full_update();
    EXPECT_TRUE(updates.get_bomb().has_value());
    EXPECT_EQ(updates.get_bomb().value().item.get_bomb_phase(), BombPhaseType::Exploded);
    EXPECT_LE(updates.get_players().at("tt").get_health(), PlayerConfig::full_health);
    EXPECT_LE(updates.get_players().at("ct").get_health(), PlayerConfig::full_health);
    EXPECT_EQ(updates.get_phase().get_phase(), PhaseType::RoundEnd);
}

TEST_F(TestGameBomb, PlayerCannotDefuseBombIfItIsNotPlanted) {
    Message msg_start_defusing = Message(StartDefusingBombCommand());
    game.tick({PlayerMessage("ct", msg_start_defusing)});
    GameUpdate updates = game.get_full_update();

    EXPECT_TRUE(updates.get_players().at("tt").get_inventory().get_bomb().has_value());
    const BombUpdate& bomb_update =
            updates.get_players().at("tt").get_inventory().get_bomb().value();
    EXPECT_EQ(bomb_update.get_bomb_phase(), BombPhaseType::NotPlanted);
}

TEST_F(TestGameBomb, PlayerDefuseBombAfterDefusingForSecondsToDefuseTime) {
    SetUpBombPlanted();

    advance_secs(BombConfig::secs_to_explode - 1 - BombConfig::secs_to_defuse);
    Message msg_start_defusing = Message(StartDefusingBombCommand());
    game.tick({PlayerMessage("ct", msg_start_defusing)});
    advance_secs(BombConfig::secs_to_defuse);
    advance_secs(PhaseTimes::round_end_duration / 2.0f);

    auto player_messages = game.tick({});

    bool found_bomb_defused_resp = false;
    bool found_round_end_resp = false;
    for (const auto& player_msg: player_messages) {
        if (player_msg.get_message().get_type() ==
            MessageType::BOMB_DEFUSED_RESP) {  // cppcheck-suppress[useStlAlgorithm]
            found_bomb_defused_resp = true;
        } else if (player_msg.get_message().get_type() ==
                   MessageType::ROUND_END_RESP) {  // cppcheck-suppress[useStlAlgorithm]
            found_round_end_resp = true;
            auto round_end_resp = player_msg.get_message().get_content<RoundEndResponse>();
            EXPECT_EQ(round_end_resp.get_winning_team(), Team::CT);
        }
    }
    EXPECT_TRUE(found_bomb_defused_resp);
    EXPECT_TRUE(found_round_end_resp);

    GameUpdate updates = game.get_full_update();
    EXPECT_TRUE(updates.get_bomb().has_value());
    EXPECT_EQ(updates.get_bomb().value().item.get_bomb_phase(), BombPhaseType::Defused);
    EXPECT_EQ(updates.get_players().at("tt").get_health(), PlayerConfig::full_health);
    EXPECT_EQ(updates.get_players().at("ct").get_health(), PlayerConfig::full_health);
    EXPECT_EQ(updates.get_phase().get_phase(), PhaseType::RoundEnd);
}

TEST_F(TestGameBomb, BombExplodeIfReachingSecondsToExplodeBeforeDefusing) {
    SetUpBombPlanted();

    advance_secs(BombConfig::secs_to_explode - 1);
    Message msg_start_defusing = Message(StartDefusingBombCommand());
    game.tick({PlayerMessage("ct", msg_start_defusing)});
    advance_secs(1);

    auto player_messages = game.tick({});
    bool found_bomb_exploded_resp = false;
    for (const auto& player_msg: player_messages) {
        if (player_msg.get_message().get_type() ==
            MessageType::BOMB_EXPLODED_RESP) {  // cppcheck-suppress[useStlAlgorithm]
            found_bomb_exploded_resp = true;
        }
    }
    EXPECT_TRUE(found_bomb_exploded_resp);

    advance_secs(BombConfig::secs_to_defuse);
    bool found_bomb_defused_resp = false;
    for (const auto& player_msg: player_messages) {
        if (player_msg.get_message().get_type() ==
            MessageType::BOMB_DEFUSED_RESP) {  // cppcheck-suppress[useStlAlgorithm]
            found_bomb_defused_resp = true;
        }
    }
    EXPECT_FALSE(found_bomb_defused_resp);

    GameUpdate updates = game.get_full_update();
    EXPECT_TRUE(updates.get_bomb().has_value());
    EXPECT_EQ(updates.get_bomb().value().item.get_bomb_phase(), BombPhaseType::Exploded);
    EXPECT_LE(updates.get_players().at("tt").get_health(), PlayerConfig::full_health);
    EXPECT_LE(updates.get_players().at("ct").get_health(), PlayerConfig::full_health);
    EXPECT_EQ(updates.get_phase().get_phase(), PhaseType::RoundEnd);
}

TEST_F(TestGameBomb, PlayerDoesNotDefuseBombIfStopDefusingBeforeSecondsToDefuseTime) {
    SetUpBombPlanted();

    Message msg_start_defusing = Message(StartDefusingBombCommand());
    game.tick({PlayerMessage("ct", msg_start_defusing)});

    advance_secs(BombConfig::secs_to_defuse / 2.0f);
    Message msg_stop_defusing = Message(StopDefusingBombCommand());
    auto player_messages = game.tick({PlayerMessage("ct", msg_stop_defusing)});

    GameUpdate updates = player_messages[0].get_message().get_content<GameUpdate>();
    EXPECT_TRUE(updates.get_bomb().has_value());
    EXPECT_EQ(updates.get_bomb().value().item.get_bomb_phase(), BombPhaseType::Planted);
    EXPECT_EQ(game.get_full_update().get_phase().get_phase(), PhaseType::BombPlanted);
}

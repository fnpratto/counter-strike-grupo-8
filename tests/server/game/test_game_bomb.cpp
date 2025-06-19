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
            max_players(map.max_players),
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
}

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

    void advance_secs(float secs) { clock->advance(std::chrono::duration<float>(secs)); }
};

TEST_F(TestGameBomb, PlayerCannotPlantBombWhenNotInPlayingPhase) {
    game.join_player("tt");

    Message msg_select_team = Message(SelectTeamCommand(Team::TT));
    Message msg_set_ready = Message(SetReadyCommand());
    game.tick({PlayerMessage("tt", msg_select_team), PlayerMessage("tt", msg_set_ready)});

    Message msg_start_planting = Message(StartPlantingBombCommand());
    game.tick({PlayerMessage("tt", msg_start_planting)});
    GameUpdate updates = game.get_full_update();

    EXPECT_TRUE(updates.get_players().at("tt").get_inventory().get_bomb().has_value());
    const BombUpdate& bomb = updates.get_players().at("tt").get_inventory().get_bomb().value();
    EXPECT_EQ(bomb.get_bomb_phase(), BombPhaseType::NotPlanted);
}

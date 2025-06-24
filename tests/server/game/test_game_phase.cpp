#include <gtest/gtest.h>

#include "common/models.h"
#include "common/updates/game_update.h"
#include "common/updates/gun_update.h"
#include "common/updates/inventory_update.h"
#include "common/updates/phase_update.h"
#include "common/updates/player_update.h"
#include "server/game/game_config.h"
#include "server/game/game_phase.h"

#include "mock_clock.h"

class TestGamePhase: public ::testing::Test {
protected:
    GameConfig config;
    std::shared_ptr<MockClock> clock;
    GamePhase game_phase;

    TestGamePhase():
            config(GameConfig::load_config("./server/config.yaml")),
            clock(std::make_shared<MockClock>(std::chrono::steady_clock::now())),
            game_phase(clock, config.phase_times) {}

    void advance_secs(int secs) { clock->advance(std::chrono::seconds(secs)); }
};

TEST_F(TestGamePhase, NotStartedWhenInitialized) { EXPECT_FALSE(game_phase.is_playing()); }

TEST_F(TestGamePhase, StartInBuyingPhase) {
    game_phase.start_game();
    PhaseUpdate updates = game_phase.get_updates();
    EXPECT_EQ(updates.get_type(), PhaseType::Buying);
}

TEST_F(TestGamePhase, StartPlayingAfterBuyingDuration) {
    game_phase.start_game();
    advance_secs(config.phase_times.buying_duration);
    game_phase.advance();

    PhaseUpdate updates = game_phase.get_updates();
    EXPECT_EQ(updates.get_type(), PhaseType::InRound);
}

TEST_F(TestGamePhase, FinishOneRoundAfterRoundDuration) {
    game_phase.start_game();

    advance_secs(config.phase_times.buying_duration);
    game_phase.advance();
    advance_secs(config.phase_times.round_duration);
    game_phase.advance();

    PhaseUpdate updates = game_phase.get_updates();
    EXPECT_EQ(updates.get_type(), PhaseType::RoundEnd);
}

TEST_F(TestGamePhase, StartAnotherRoundAfterRoundFinishedDuration) {
    game_phase.start_game();

    advance_secs(config.phase_times.buying_duration);
    game_phase.advance();
    advance_secs(config.phase_times.round_duration);
    game_phase.advance();
    advance_secs(config.phase_times.round_end_duration);
    game_phase.advance();

    PhaseUpdate updates = game_phase.get_updates();
    EXPECT_EQ(updates.get_type(), PhaseType::Buying);
}

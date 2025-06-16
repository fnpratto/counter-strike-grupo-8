#include <gtest/gtest.h>

#include "common/models.h"
#include "common/updates/game_update.h"
#include "common/updates/gun_update.h"
#include "common/updates/inventory_update.h"
#include "common/updates/phase_update.h"
#include "common/updates/player_update.h"
#include "server/clock/mock_clock.h"
#include "server/game/game_config.h"
#include "server/game/game_phase.h"

class TestGamePhase: public ::testing::Test {
protected:
    std::shared_ptr<MockClock> clock;
    GamePhase game_phase;

    TestGamePhase():
            clock(std::make_shared<MockClock>(std::chrono::steady_clock::now())),
            game_phase(clock) {}

    void advance_secs(int secs) { clock->advance(std::chrono::seconds(secs)); }
};

TEST_F(TestGamePhase, NotStartedWhenInitialized) { EXPECT_FALSE(game_phase.is_started()); }

TEST_F(TestGamePhase, StartInBuyingPhase) {
    game_phase.start_game();
    PhaseUpdate updates = game_phase.get_updates();
    EXPECT_EQ(updates.get_phase(), PhaseType::Buying);
}

TEST_F(TestGamePhase, StartPlayingAfterBuyingDuration) {
    game_phase.start_game();
    advance_secs(PhaseTimes::buying_phase_secs);
    game_phase.advance();

    PhaseUpdate updates = game_phase.get_updates();
    EXPECT_EQ(updates.get_phase(), PhaseType::Playing);
}

TEST_F(TestGamePhase, FinishOneRoundAfterRoundDuration) {
    game_phase.start_game();

    advance_secs(PhaseTimes::buying_phase_secs);
    game_phase.advance();
    advance_secs(PhaseTimes::playing_phase_secs);
    game_phase.advance();

    PhaseUpdate updates = game_phase.get_updates();
    EXPECT_EQ(updates.get_phase(), PhaseType::RoundEnd);
}

TEST_F(TestGamePhase, StartAnotherRoundAfterRoundFinishedDuration) {
    game_phase.start_game();

    advance_secs(PhaseTimes::buying_phase_secs);
    game_phase.advance();
    advance_secs(PhaseTimes::playing_phase_secs);
    game_phase.advance();
    advance_secs(PhaseTimes::round_end_phase_secs);
    game_phase.advance();

    PhaseUpdate updates = game_phase.get_updates();
    EXPECT_EQ(updates.get_phase(), PhaseType::Buying);
}

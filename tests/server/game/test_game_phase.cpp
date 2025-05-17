#include <gtest/gtest.h>

#include "common/models.h"
#include "server/clock/mock_clock.h"
#include "server/game/game_phase.h"
#include "server/cons.h"

using namespace PhaseTimes;

class TestGamePhase : public ::testing::Test {
protected:
    MockClock clock;
    GamePhase game_phase;

    TestGamePhase() : 
            clock(std::chrono::steady_clock::now()), 
            game_phase(clock) {}

    void advance_secs(int secs) {
        clock.advance(std::chrono::seconds(secs));
    }
};

TEST_F(TestGamePhase, NotStartedWhenInitialized) {
    EXPECT_FALSE(game_phase.is_started());
}

TEST_F(TestGamePhase, StartInBuyingPhase) {
    game_phase.start_buying_phase();
    EXPECT_TRUE(game_phase.is_buying_phase());
}

TEST_F(TestGamePhase, StartPlayingAfterBuyingDuration) {
    game_phase.start_buying_phase();
    advance_secs(buying_phase_secs);
    game_phase.update();
    EXPECT_TRUE(game_phase.is_started());
    EXPECT_FALSE(game_phase.is_buying_phase());
    EXPECT_FALSE(game_phase.is_round_finished());
}

TEST_F(TestGamePhase, FinishOneRoundAfterRoundDuration) {
    game_phase.start_buying_phase();

    advance_secs(buying_phase_secs);
    game_phase.update();
    advance_secs(playing_phase_secs);
    game_phase.update();

    EXPECT_TRUE(game_phase.is_round_finished());
}

TEST_F(TestGamePhase, StartAnotherRoundAfterRoundFinishedDuration) {
    game_phase.start_buying_phase();

    advance_secs(buying_phase_secs);
    game_phase.update();
    advance_secs(playing_phase_secs);
    game_phase.update();
    advance_secs(round_finished_phase_secs);
    game_phase.update();

    EXPECT_TRUE(game_phase.is_buying_phase());
}

#include <gtest/gtest.h>

#include "server/game/game.h"
#include "common/models.h"
#include "server/game/clock/mock_clock.h"

#define BUYING_PHASE_SECS 30
#define PLAYING_PHASE_SECS 120

class TestGame : public ::testing::Test {
protected:
    MockClock clock;
    Game game;

    TestGame() : 
            clock(std::chrono::steady_clock::now()), 
            game("test_match", clock) {}

    void advance_secs(int secs) {
        clock.advance(std::chrono::seconds(secs));
    }
};

TEST_F(TestGame, StartEmpty) {
    EXPECT_EQ(game.get_num_players(), 0);
    EXPECT_FALSE(game.is_started());
    EXPECT_FALSE(game.is_full());
    EXPECT_EQ(game.get_phase_type(), GamePhaseType::NotStarted);
}

TEST_F(TestGame, PlayerCanJoinGame) {
    game.join("test_player");
    EXPECT_EQ(game.get_num_players(), 1);
}

TEST_F(TestGame, PlayerCannotJoinGameTwice) {
    game.join("test_player");
    EXPECT_THROW({
        game.join("test_player");
    }, JoinGameError);
    EXPECT_EQ(game.get_num_players(), 1);
}

TEST_F(TestGame, PlayerCannotJoinGameWithInvalidName) {
    EXPECT_THROW({
        game.join("");
    }, JoinGameError);
    EXPECT_EQ(game.get_num_players(), 0);
}

TEST_F(TestGame, PlayerCannotJoinFullGame) {
    for (int i = 0; i < game.get_max_num_players(); i++) {
        game.join("test_player_" + std::to_string(i));
    }
    EXPECT_THROW({
        game.join("extra_player");
    }, JoinGameError);
    EXPECT_EQ(game.get_num_players(), game.get_max_num_players());
}

TEST_F(TestGame, StartInBuyingPhase) {
    game.start();
    EXPECT_EQ(game.get_phase_type(), GamePhaseType::Buying);
}

TEST_F(TestGame, StartPlayingAfterBuyingDuration) {
    game.start();
    
    advance_secs(game.get_buying_phase_duration() + 1);
    game.tick();

    EXPECT_EQ(game.get_phase_type(), GamePhaseType::Playing);
}

TEST_F(TestGame, FinishAfterRoundDuration) {
    game.start();
    
    advance_secs(game.get_buying_phase_duration() + 1);
    game.tick();
    advance_secs(game.get_playing_phase_duration() + 1);
    game.tick();

    EXPECT_EQ(game.get_phase_type(), GamePhaseType::Finished);
}

TEST_F(TestGame, PlayerCannotJoinStartedGame) {
    game.start();
    EXPECT_THROW({
        game.join("test_player");
    }, JoinGameError);
    EXPECT_EQ(game.get_num_players(), 0);
}

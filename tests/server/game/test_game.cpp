#include <gtest/gtest.h>

#include "common/models.h"
#include "server/game/game.h"
#include "server/game/game_config.h"
#include "server/clock/mock_clock.h"

class TestGame : public ::testing::Test {
protected:
    MockClock clock;
    GameConfig config;
    Shop shop;
    Game game;

    TestGame() : 
            clock(std::chrono::steady_clock::now()), 
            config(),
            game("test_game", clock, config, shop) {}

    void advance_secs(int secs) {
        clock.advance(std::chrono::seconds(secs));
    }
};

TEST_F(TestGame, StartEmpty) {
    EXPECT_EQ(game.get_num_players(), 0);
    EXPECT_FALSE(game.is_started());
    EXPECT_FALSE(game.is_full());
    EXPECT_EQ(game.get_phase_type(), RoundPhaseType::NotStarted);
}

TEST_F(TestGame, PlayerCanJoinGame) {
    game.join("test_player", Team::Terrorist);
    EXPECT_EQ(game.get_num_players(), 1);
}

TEST_F(TestGame, PlayerCannotJoinGameTwice) {
    game.join("test_player", Team::Terrorist);
    EXPECT_THROW({
        game.join("test_player", Team::Terrorist);
    }, JoinGameError);
    EXPECT_EQ(game.get_num_players(), 1);
}

TEST_F(TestGame, PlayerCannotJoinGameWithInvalidName) {
    EXPECT_THROW({
        game.join("", Team::Terrorist);
    }, JoinGameError);
    EXPECT_EQ(game.get_num_players(), 0);
}

TEST_F(TestGame, PlayerCannotJoinFullTeam) {
    for (unsigned int i = 1; i <= config.get_max_players_team(); i++) {
        game.join("test_player_" + std::to_string(i), Team::Terrorist);
    }
    EXPECT_THROW({
        game.join("extra_player", Team::Terrorist);
    }, JoinGameError);
    EXPECT_EQ(game.get_num_players(), config.get_max_players_team());
}

TEST_F(TestGame, PlayerCannotJoinFullGame) {
    for (unsigned int i = 1; i <= config.get_max_players_team(); i++) {
        game.join("test_terrorist_" + std::to_string(i), Team::Terrorist);
    }
    for (unsigned int i = 1; i <= config.get_max_players_team(); i++) {
        game.join("test_counter_terrorist_" + std::to_string(i), Team::CounterTerrorist);
    }
    EXPECT_THROW({
        game.join("extra_player", Team::Terrorist);
    }, JoinGameError);
    EXPECT_EQ(game.get_num_players(), config.get_max_players_game());
}

TEST_F(TestGame, StartInBuyingPhase) {
    game.start();
    EXPECT_EQ(game.get_phase_type(), RoundPhaseType::Buying);
}

TEST_F(TestGame, CannotStartAnAlreadyStartedGame) {
    game.start();
    EXPECT_THROW({
        game.start();
    }, StartGameError);
}

TEST_F(TestGame, StartPlayingAfterBuyingDuration) {
    game.start();
    
    advance_secs(config.get_buying_phase_secs());
    game.tick();

    EXPECT_EQ(game.get_phase_type(), RoundPhaseType::Playing);
}

TEST_F(TestGame, PlayerCannotJoinStartedGame) {
    game.start();
    EXPECT_THROW({
        game.join("test_player", Team::Terrorist);
    }, JoinGameError);
    EXPECT_EQ(game.get_num_players(), 0);
}

TEST_F(TestGame, FinishOneRoundAfterRoundDuration) {
    game.start();
    EXPECT_EQ(game.get_num_rounds_played(), 0);

    advance_secs(config.get_buying_phase_secs());
    game.tick();
    advance_secs(config.get_playing_phase_secs());
    game.tick();

    EXPECT_EQ(game.get_phase_type(), RoundPhaseType::Finished);
    EXPECT_EQ(game.get_num_rounds_played(), 1);
}

TEST_F(TestGame, StartAnotherRoundAfterFinishingOneRound) {
    game.start();

    advance_secs(config.get_buying_phase_secs());
    game.tick();
    advance_secs(config.get_playing_phase_secs());
    game.tick();
    advance_secs(1);
    game.tick();

    EXPECT_EQ(game.get_phase_type(), RoundPhaseType::Buying);
}

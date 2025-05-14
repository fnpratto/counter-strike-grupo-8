#include <gtest/gtest.h>

#include "common/models.h"
#include "server/clock/mock_clock.h"
#include "server/game/game.h"
#include "server/game/shop.h"
#include "server/game/game_config.h"
#include "server/errors.h"

class TestGame : public ::testing::Test {
protected:
    MockClock clock;
    Game game;

    TestGame() : 
            clock(std::chrono::steady_clock::now()), 
            game("test_game", clock, GameConfig(), Shop()) {}

    void advance_secs(int secs) {
        clock.advance(std::chrono::seconds(secs));
    }
};

TEST_F(TestGame, StartEmpty) {
    GameConfig conf = game.get_config();
    PhaseType phase = game.get_phase();
    EXPECT_EQ(conf.get_num_players(), 0);
    EXPECT_EQ(phase, PhaseType::NotStarted);
    EXPECT_FALSE(conf.is_full());
}

TEST_F(TestGame, PlayerCanJoinGame) {
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, "test_player");
    EXPECT_EQ(game.get_config().get_num_players(), 1);
}

TEST_F(TestGame, PlayerCannotJoinGameTwice) {
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, "test_player");
    EXPECT_THROW({
        game.tick(msg_join, "test_player");
    }, JoinGameError);
    EXPECT_EQ(game.get_config().get_num_players(), 1);
}

TEST_F(TestGame, PlayerCannotJoinGameWithInvalidName) {
    Message msg_join = Message(JoinGameCommand(""));
    EXPECT_THROW({
        game.tick(msg_join, "");
    }, InvalidPlayerNameError);
    EXPECT_EQ(game.get_config().get_num_players(), 0);
}

TEST_F(TestGame, PlayersCanJoinGameUntilItIsFull) {
    Message msg_join = Message(JoinGameCommand(""));
    for (unsigned int i = 1; i <= game.get_config().get_max_players(); i++) {
        game.tick(msg_join, "test_player_" + std::to_string(i));
    }
    EXPECT_THROW({
        game.tick(msg_join, "extra_player");
    }, JoinGameError);
    EXPECT_EQ(game.get_config().get_num_players(), game.get_config().get_max_players());
}

TEST_F(TestGame, PlayerCanSelectTeam) {
    Message msg_join = Message(JoinGameCommand(""));
    Message msg_select_team = Message(SelectTeamCommand(Team::Terrorist));
    game.tick(msg_join, "test_player");
    game.tick(msg_select_team, "test_player");
    EXPECT_EQ(game.get_config().get_num_players(), 1);
}

TEST_F(TestGame, PlayerCannotSelectFullTeam) {
    Message msg_join = Message(JoinGameCommand(""));
    Message msg_select_team = Message(SelectTeamCommand(Team::Terrorist));
    for (unsigned int i = 1; i <= game.get_config().get_max_players_per_team(); i++) {
        game.tick(msg_join, "test_player_" + std::to_string(i));
        game.tick(msg_select_team, "test_player_" + std::to_string(i));
    }
    
    game.tick(msg_join, "extra_player");
    EXPECT_THROW({
        game.tick(msg_select_team, "extra_player");
    }, SelectTeamError);
    EXPECT_EQ(game.get_config().get_num_players(), game.get_config().get_max_players_per_team() + 1);
}

TEST_F(TestGame, StartInBuyingPhase) {
    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "test_player");
    EXPECT_EQ(game.get_phase(), PhaseType::Buying);
}

TEST_F(TestGame, CannotStartAnAlreadyStartedGame) {
    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "test_player");
    EXPECT_THROW({
        game.tick(msg_start, "test_player");
    }, StartGameError);
}

TEST_F(TestGame, StartPlayingAfterBuyingDuration) {
    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "test_player");
    
    advance_secs(game.get_config().get_buying_phase_secs());
    game.tick(Message(), "test_player");

    EXPECT_EQ(game.get_phase(), PhaseType::Playing);
}

TEST_F(TestGame, PlayerCannotJoinStartedGame) {
    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "test_player");

    Message msg_join = Message(JoinGameCommand(""));
    EXPECT_THROW({
        game.tick(msg_join, "test_player");
    }, JoinGameError);
    EXPECT_EQ(game.get_config().get_num_players(), 0);
}

TEST_F(TestGame, FinishOneRoundAfterRoundDuration) {
    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "test_player");
    EXPECT_EQ(game.get_config().get_num_rounds(), 0);

    advance_secs(game.get_config().get_buying_phase_secs());
    game.tick(Message(), "test_player");
    advance_secs(game.get_config().get_playing_phase_secs());
    game.tick(Message(), "test_player");

    EXPECT_EQ(game.get_phase(), PhaseType::RoundFinished);
    EXPECT_EQ(game.get_config().get_num_rounds(), 1);
}

TEST_F(TestGame, StartAnotherRoundAfterFinishingOneRound) {
    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "test_player");

    advance_secs(game.get_config().get_buying_phase_secs());
    game.tick(Message(), "test_player");
    advance_secs(game.get_config().get_playing_phase_secs());
    game.tick(Message(), "test_player");
    advance_secs(1);
    game.tick(Message(), "test_player");

    EXPECT_EQ(game.get_phase(), PhaseType::Buying);
}

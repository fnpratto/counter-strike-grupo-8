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
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, "test_player");
    EXPECT_EQ(game.get_num_players(), 1);
}

TEST_F(TestGame, PlayerCannotJoinGameTwice) {
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, "test_player");
    EXPECT_THROW({
        game.tick(msg_join, "test_player");
    }, JoinGameError);
    EXPECT_EQ(game.get_num_players(), 1);
}

TEST_F(TestGame, PlayerCannotJoinGameWithInvalidName) {
    Message msg_join = Message(JoinGameCommand(""));
    EXPECT_THROW({
        game.tick(msg_join, "");
    }, JoinGameError);
    EXPECT_EQ(game.get_num_players(), 0);
}

TEST_F(TestGame, PlayersCanJoinGameUntilItIsFull) {
    Message msg_join = Message(JoinGameCommand(""));
    for (unsigned int i = 1; i <= config.get_max_players_game(); i++) {
        game.tick(msg_join, "test_player_" + std::to_string(i));
    }
    EXPECT_THROW({
        game.tick(msg_join, "extra_player");
    }, JoinGameError);
    EXPECT_EQ(game.get_num_players(), config.get_max_players_game());
}

TEST_F(TestGame, PlayerCanSelectTeam) {
    Message msg_join = Message(JoinGameCommand(""));
    Message msg_select_team = Message(SelectTeamCommand(Team::Terrorist));
    game.tick(msg_join, "test_player");
    game.tick(msg_select_team, "test_player");
    EXPECT_EQ(game.get_num_players(), 1);
}

TEST_F(TestGame, PlayerCannotSelectFullTeam) {
    Message msg_join = Message(JoinGameCommand(""));
    Message msg_select_team = Message(SelectTeamCommand(Team::Terrorist));
    for (unsigned int i = 1; i <= config.get_max_players_team(); i++) {
        game.tick(msg_join, "test_player_" + std::to_string(i));
        game.tick(msg_select_team, "test_player_" + std::to_string(i));
    }
    
    game.tick(msg_join, "extra_player");
    EXPECT_THROW({
        game.tick(msg_select_team, "extra_player");
    }, SelectTeamError);
    EXPECT_EQ(game.get_num_players(), config.get_max_players_team() + 1);
}

TEST_F(TestGame, StartInBuyingPhase) {
    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "");
    EXPECT_EQ(game.get_phase_type(), RoundPhaseType::Buying);
}

TEST_F(TestGame, CannotStartAnAlreadyStartedGame) {
    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "");
    EXPECT_THROW({
        game.tick(msg_start, "");
    }, StartGameError);
}

TEST_F(TestGame, StartPlayingAfterBuyingDuration) {
    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "");
    
    advance_secs(config.get_buying_phase_secs());
    game.tick(Message(), "");

    EXPECT_EQ(game.get_phase_type(), RoundPhaseType::Playing);
}

TEST_F(TestGame, PlayerCannotJoinStartedGame) {
    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "");

    Message msg_join = Message(JoinGameCommand(""));
    EXPECT_THROW({
        game.tick(msg_join, "test_player");
    }, JoinGameError);
    EXPECT_EQ(game.get_num_players(), 0);
}

TEST_F(TestGame, FinishOneRoundAfterRoundDuration) {
    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "");
    EXPECT_EQ(game.get_num_rounds_played(), 0);

    advance_secs(config.get_buying_phase_secs());
    game.tick(Message(), "");
    advance_secs(config.get_playing_phase_secs());
    game.tick(Message(), "");

    EXPECT_EQ(game.get_phase_type(), RoundPhaseType::Finished);
    EXPECT_EQ(game.get_num_rounds_played(), 1);
}

TEST_F(TestGame, StartAnotherRoundAfterFinishingOneRound) {
    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "");

    advance_secs(config.get_buying_phase_secs());
    game.tick(Message(), "");
    advance_secs(config.get_playing_phase_secs());
    game.tick(Message(), "");
    advance_secs(1);
    game.tick(Message(), "");

    EXPECT_EQ(game.get_phase_type(), RoundPhaseType::Buying);
}

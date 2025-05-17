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
    GameConfig conf;
    Game game;

    TestGame() : 
            clock(std::chrono::steady_clock::now()), 
            game("test_game", clock, conf, Shop()) {}

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
    for (int i = 1; i <= game.get_config().get_max_players(); i++) {
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
    for (int i = 1; i <= game.get_config().get_max_team_players(); i++) {
        game.tick(msg_join, "test_player_" + std::to_string(i));
        game.tick(msg_select_team, "test_player_" + std::to_string(i));
    }
    
    game.tick(msg_join, "extra_player");
    EXPECT_THROW({
        game.tick(msg_select_team, "extra_player");
    }, SelectTeamError);
    EXPECT_EQ(game.get_config().get_num_players(), game.get_config().get_max_team_players() + 1);
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

TEST_F(TestGame, PlayerCannotSelectTeamWhenStartedGame) {
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, "test_player");
    Message msg_select_team = Message(SelectTeamCommand(Team::Terrorist));
    game.tick(msg_select_team, "test_player");
    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "test_player");
    
    msg_select_team = Message(SelectTeamCommand(Team::CounterTerrorist));
    EXPECT_THROW({
        game.tick(msg_select_team, "test_player");
    }, SelectTeamError);
    EXPECT_EQ(game.get_config().get_num_tts(), 1);
    EXPECT_EQ(game.get_config().get_num_cts(), 0);
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

TEST_F(TestGame, StartAnotherRoundAfterRoundFinishedDuration) {
    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "test_player");

    advance_secs(game.get_config().get_buying_phase_secs());
    game.tick(Message(), "test_player");
    advance_secs(game.get_config().get_playing_phase_secs());
    game.tick(Message(), "test_player");
    advance_secs(game.get_config().get_round_finished_phase_secs());
    game.tick(Message(), "test_player");

    EXPECT_EQ(game.get_phase(), PhaseType::Buying);
}

TEST_F(TestGame, PlayersSwapTeamsAfterHalfOfMaxRounds) {
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, "test_player1");
    Message msg_select_team = Message(SelectTeamCommand(Team::Terrorist));
    game.tick(msg_select_team, "test_player1");

    game.tick(msg_join, "test_player2");
    msg_select_team = Message(SelectTeamCommand(Team::CounterTerrorist));
    game.tick(msg_select_team, "test_player2");

    Message msg_start = Message(StartGameCommand());
    game.tick(msg_start, "test_player1");

    for (int i = 1; i <= game.get_config().get_max_rounds() / 2; i++) {
        advance_secs(game.get_config().get_buying_phase_secs());
        game.tick(Message(), "test_player1");
        advance_secs(game.get_config().get_playing_phase_secs());
        game.tick(Message(), "test_player1");
        advance_secs(game.get_config().get_round_finished_phase_secs());
        game.tick(Message(), "test_player");
    }

    EXPECT_TRUE(game.get_config().get_player("test_player1").is_ct());
    EXPECT_TRUE(game.get_config().get_player("test_player2").is_tt());
}

TEST_F(TestGame, PlayerCanMove) {
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, "test_player");
    
    advance_secs(game.get_config().get_buying_phase_secs());
    game.tick(Message(), "test_player");

    Player player = game.get_config().get_player("test_player");
    float old_pos_x = player.get_pos_x();
    float old_pos_y = player.get_pos_y();
    
    Message msg_move = Message(MoveCommand(0, 1));
    game.tick(msg_move, "test_player");

    player = game.get_config().get_player("test_player");
    float new_pos_x = player.get_pos_x();
    float new_pos_y = player.get_pos_y();
    
    EXPECT_EQ(new_pos_x, old_pos_x);
    EXPECT_EQ(new_pos_y, old_pos_y + 1 * conf.get_player_speed() * (1 / conf.get_tickrate()));
}

TEST_F(TestGame, PlayerCanMoveInDiagonal) {
    Message msg_join = Message(JoinGameCommand(""));
    game.tick(msg_join, "test_player");
    
    advance_secs(game.get_config().get_buying_phase_secs());
    game.tick(Message(), "test_player");

    Player player = game.get_config().get_player("test_player");
    float old_pos_x = player.get_pos_x();
    float old_pos_y = player.get_pos_y();
    
    Vector2D dir(1, 1);
    Message msg_move = Message(MoveCommand(1, 1));
    game.tick(msg_move, "test_player");

    player = game.get_config().get_player("test_player");
    float new_pos_x = player.get_pos_x();
    float new_pos_y = player.get_pos_y();
    
    Vector2D step = dir.normalize() * conf.get_player_speed() * (1 / conf.get_tickrate());
    EXPECT_EQ(new_pos_x, old_pos_x + step.get_x());
    EXPECT_EQ(new_pos_y, old_pos_y + step.get_y());
}

#include <gtest/gtest.h>

#include "common/models.h"
#include "server/clock/mock_clock.h"
#include "server/errors.h"
#include "server/game/game.h"
#include "server/game/game_config.h"
#include "server/game/shop.h"
#include "server/map/map.h"
#include "server/map/map_builder.h"

// using GameConfig::tickrate;
// using PhaseTimes::buying_duration;
// using PhaseTimes::round_duration;

// class TestGame: public ::testing::Test {
// protected:
//     MockClock clock;
//     Map map;
//     Game game;

//     TestGame():
//             clock(std::chrono::steady_clock::now()),
//             map(MapBuilder("tests/server/map/map.yaml").build()),
//             game("test_game", clock, Shop(), map) {}

//     void advance_secs(int secs) { clock.advance(std::chrono::seconds(secs)); }
// };

// TEST_F(TestGame, PlayerCanJoinGame) {
//     Message msg_join = Message(JoinGameCommand(""));
//     game.tick(msg_join, "test_player");
//     EXPECT_EQ(game.get_game_state().players.size(), 1);
// }

// TEST_F(TestGame, PlayerCannotJoinGameTwice) {
//     Message msg_join = Message(JoinGameCommand(""));
//     game.tick(msg_join, "test_player");
//     EXPECT_THROW({ game.tick(msg_join, "test_player"); }, JoinGameError);
//     EXPECT_EQ(game.get_game_state().players.size(), 1);
// }

// TEST_F(TestGame, PlayerCannotJoinGameWithInvalidName) {
//     Message msg_join = Message(JoinGameCommand(""));
//     EXPECT_THROW({ game.tick(msg_join, ""); }, InvalidPlayerNameError);
//     EXPECT_EQ(game.get_game_state().players.size(), 0);
// }

// TEST_F(TestGame, PlayersCanJoinGameUntilItIsFull) {
//     Message msg_join = Message(JoinGameCommand(""));
//     for (int i = 1; i <= max_players; i++) {
//         game.tick(msg_join, "test_player_" + std::to_string(i));
//     }
//     EXPECT_THROW({ game.tick(msg_join, "extra_player"); }, JoinGameError);
//     EXPECT_EQ(game.get_game_state().players.size(), max_players);
// }

// TEST_F(TestGame, PlayerCanSelectTeam) {
//     Message msg_join = Message(JoinGameCommand(""));
//     Message msg_select_team = Message(SelectTeamCommand(Team::Terrorist));
//     game.tick(msg_join, "test_player");
//     game.tick(msg_select_team, "test_player");
//     EXPECT_EQ(game.get_game_state().players.size(), 1);
// }

// TEST_F(TestGame, PlayerCannotSelectFullTeam) {
//     Message msg_join = Message(JoinGameCommand(""));
//     Message msg_select_team = Message(SelectTeamCommand(Team::Terrorist));
//     for (int i = 1; i <= max_team_players; i++) {
//         game.tick(msg_join, "test_player_" + std::to_string(i));
//         game.tick(msg_select_team, "test_player_" + std::to_string(i));
//     }

//     game.tick(msg_join, "extra_player");
//     EXPECT_THROW({ game.tick(msg_select_team, "extra_player"); }, SelectTeamError);
//     EXPECT_EQ(game.get_game_state().players.size(), max_team_players + 1);
// }

// TEST_F(TestGame, CannotStartAnAlreadyStartedGame) {
//     Message msg_start = Message(StartGameCommand());
//     game.tick(msg_start, "test_player");
//     EXPECT_THROW({ game.tick(msg_start, "test_player"); }, StartGameError);
// }

// TEST_F(TestGame, PlayerCannotJoinStartedGame) {
//     Message msg_start = Message(StartGameCommand());
//     game.tick(msg_start, "test_player");

//     Message msg_join = Message(JoinGameCommand(""));
//     EXPECT_THROW({ game.tick(msg_join, "test_player"); }, JoinGameError);
//     EXPECT_EQ(game.get_game_state().players.size(), 0);
// }

// TEST_F(TestGame, PlayerCannotSelectTeamWhenStartedGame) {
//     Message msg_join = Message(JoinGameCommand(""));
//     game.tick(msg_join, "test_player");
//     Message msg_select_team = Message(SelectTeamCommand(Team::Terrorist));
//     game.tick(msg_select_team, "test_player");
//     Message msg_start = Message(StartGameCommand());
//     game.tick(msg_start, "test_player");

//     msg_select_team = Message(SelectTeamCommand(Team::CounterTerrorist));
//     EXPECT_THROW({ game.tick(msg_select_team, "test_player"); }, SelectTeamError);
//     EXPECT_EQ(game.get_game_state().num_tts, 1);
//     EXPECT_EQ(game.get_game_state().num_cts, 0);
// }

// TEST_F(TestGame, IncrementRoundsPlayedAfterRoundDuration) {
//     Message msg_start = Message(StartGameCommand());
//     game.tick(msg_start, "test_player");
//     EXPECT_EQ(game.get_game_state().num_rounds, 0);

//     advance_secs(buying_phase_secs);
//     game.tick(Message(), "test_player");
//     advance_secs(playing_phase_secs);
//     game.tick(Message(), "test_player");

//     EXPECT_EQ(game.get_game_state().num_rounds, 1);
// }

// TEST_F(TestGame, OneTerroristHasBombWhenGameStarted) {
//     Message msg_join = Message(JoinGameCommand(""));
//     game.tick(msg_join, "test_player");
//     Message msg_select_team = Message(SelectTeamCommand(Team::Terrorist));
//     game.tick(msg_select_team, "test_player");

//     std::string another_player_name = "another_player";
//     game.tick(msg_join, another_player_name);
//     game.tick(msg_select_team, another_player_name);

//     Message msg_start = Message(StartGameCommand());
//     game.tick(msg_start, "test_player");

//     PlayerState tt = game.get_game_state().players.at("test_player");
//     PlayerState another_tt = game.get_game_state().players.at(another_player_name);

//     if (tt.inventory.utilities.find(WeaponSlot::Bomb) != tt.inventory.utilities.end()) {
//         EXPECT_TRUE(another_tt.inventory.utilities.find(WeaponSlot::Bomb) ==
//                     another_tt.inventory.utilities.end());
//     } else if (another_tt.inventory.utilities.find(WeaponSlot::Bomb) !=
//                another_tt.inventory.utilities.end()) {
//         EXPECT_TRUE(tt.inventory.utilities.find(WeaponSlot::Bomb) ==
//         tt.inventory.utilities.end());
//     } else {
//         FAIL();
//     }
// }

// TEST_F(TestGame, CounterTerroristDoesNotHaveBombWhenGameStarted) {
//     Message msg_join = Message(JoinGameCommand(""));
//     game.tick(msg_join, "test_player");
//     Message msg_select_team = Message(SelectTeamCommand(Team::CounterTerrorist));
//     game.tick(msg_select_team, "test_player");

//     Message msg_start = Message(StartGameCommand());
//     game.tick(msg_start, "test_player");

//     InventoryState ct_invent = game.get_game_state().players.at("test_player").inventory;
//     EXPECT_TRUE(ct_invent.utilities.find(WeaponSlot::Bomb) == ct_invent.utilities.end());
// }

// TEST_F(TestGame, PlayersSwapTeamsAfterHalfOfMaxRounds) {
//     Message msg_join = Message(JoinGameCommand(""));
//     game.tick(msg_join, "test_player1");
//     Message msg_select_team = Message(SelectTeamCommand(Team::Terrorist));
//     game.tick(msg_select_team, "test_player1");

//     game.tick(msg_join, "test_player2");
//     msg_select_team = Message(SelectTeamCommand(Team::CounterTerrorist));
//     game.tick(msg_select_team, "test_player2");

//     Message msg_start = Message(StartGameCommand());
//     game.tick(msg_start, "test_player1");

//     for (int i = 1; i <= max_rounds / 2; i++) {
//         advance_secs(buying_phase_secs);
//         game.tick(Message(), "test_player1");
//         advance_secs(playing_phase_secs);
//         game.tick(Message(), "test_player1");
//         advance_secs(round_finished_phase_secs);
//         game.tick(Message(), "test_player");
//     }

//     EXPECT_EQ(game.get_game_state().players.at("test_player1").team, Team::CounterTerrorist);
//     EXPECT_EQ(game.get_game_state().players.at("test_player2").team, Team::Terrorist);
// }

// TEST_F(TestGame, PlayerCanMove) {
//     Message msg_join = Message(JoinGameCommand(""));
//     game.tick(msg_join, "test_player");

//     advance_secs(buying_phase_secs);
//     game.tick(Message(), "test_player");

//     PlayerState player = game.get_game_state().players.at("test_player");
//     float old_pos_x = player.pos_x;
//     float old_pos_y = player.pos_y;

//     Message msg_move = Message(MoveCommand(0, 1));
//     game.tick(msg_move, "test_player");

//     player = game.get_game_state().players.at("test_player");
//     float new_pos_x = player.pos_x;
//     float new_pos_y = player.pos_y;

//     EXPECT_EQ(new_pos_x, old_pos_x);
//     EXPECT_EQ(new_pos_y, old_pos_y + 1 * player_speed * (1 / tickrate));
// }

// TEST_F(TestGame, PlayerCanMoveInDiagonal) {
//     Message msg_join = Message(JoinGameCommand(""));
//     game.tick(msg_join, "test_player");

//     advance_secs(buying_phase_secs);
//     game.tick(Message(), "test_player");

//     PlayerState player = game.get_game_state().players.at("test_player");
//     float old_pos_x = player.pos_x;
//     float old_pos_y = player.pos_y;

//     Vector2D dir(1, 1);
//     Message msg_move = Message(MoveCommand(1, 1));
//     game.tick(msg_move, "test_player");

//     player = game.get_game_state().players.at("test_player");
//     float new_pos_x = player.pos_x;
//     float new_pos_y = player.pos_y;

//     Vector2D step = dir.normalized() * player_speed * (1.0f / tickrate);
//     EXPECT_EQ(new_pos_x, old_pos_x + step.get_x());
//     EXPECT_EQ(new_pos_y, old_pos_y + step.get_y());
// }

#include <gtest/gtest.h>

#include "common/game_state.h"
#include "common/models.h"
#include "common/updates/game_update.h"
#include "common/updates/inventory_update.h"
#include "common/updates/phase_update.h"
#include "common/updates/player_update.h"
#include "common/updates/utility_update.h"
#include "server/clock/mock_clock.h"
#include "server/errors.h"
#include "server/game/game.h"
#include "server/game/game_config.h"
#include "server/game/shop.h"
#include "server/map/map.h"
#include "server/map/map_builder.h"

class TestGame: public ::testing::Test {
protected:
    MockClock clock;
    Map map;
    Game game;

    TestGame():
            clock(std::chrono::steady_clock::now()),
            map(MapBuilder("../tests/server/map/map.yaml").build()),
            game("test_game", std::make_unique<MockClock>(clock), std::move(map)) {}

    void advance_secs(int secs) { clock.advance(std::chrono::seconds(secs)); }
};

TEST_F(TestGame, PlayerCanJoinGame) {
    auto& state = game.join_player("test_player");
    EXPECT_EQ(static_cast<int>(state.get_players().size()), 1);
}

TEST_F(TestGame, PlayerCannotJoinGameTwice) {
    auto& state = game.join_player("test_player");
    EXPECT_THROW({ game.join_player("test_player"); }, JoinGameError);
    EXPECT_EQ(static_cast<int>(state.get_players().size()), 1);
}

TEST_F(TestGame, PlayerCannotJoinGameWithInvalidName) {
    EXPECT_THROW({ game.join_player(""); }, InvalidPlayerNameError);
}

TEST_F(TestGame, PlayersCanJoinGameUntilItIsFull) {
    for (int i = 1; i <= GameConfig::max_players; i++) {
        game.join_player("test_player_" + std::to_string(i));
    }
    EXPECT_THROW({ game.join_player("extra_player"); }, JoinGameError);
    EXPECT_EQ(static_cast<int>(game.get_state().get_players().size()), GameConfig::max_players);
}

TEST_F(TestGame, PlayerCanSelectTeam) {
    const GameState& state = game.join_player("test_player");
    Team old_team = state.get_players().at("test_player")->get_state().get_team();

    Team new_team = Team::CT;
    if (old_team == Team::CT)
        new_team = Team::TT;

    Message msg_select_team = Message(SelectTeamCommand(new_team));
    GameUpdate updates = game.tick({msg_select_team}, "test_player");

    std::map<std::string, PlayerUpdate> player_updates = updates.get_players();
    EXPECT_EQ(player_updates.at("test_player").get_team(), new_team);
}

TEST_F(TestGame, PlayerJoinFullTeam) {
    Message msg_select_team = Message(SelectTeamCommand(Team::TT));
    for (int i = 1; i <= GameConfig::max_team_players; i++) {
        game.join_player("test_player_" + std::to_string(i));
        game.tick({msg_select_team}, "test_player_" + std::to_string(i));
    }
    auto& state = game.join_player("extra_player");
    EXPECT_EQ(static_cast<int>(state.get_players().size()), GameConfig::max_team_players + 1);

    GameUpdate updates = game.tick({msg_select_team}, "extra_player");
    EXPECT_FALSE(updates.has_players_changed());
}

TEST_F(TestGame, CannotStartAnAlreadyStartedGame) {
    game.join_player("test_player");

    GameUpdate updates = game.tick({Message(StartGameCommand())}, "test_player");
    std::map<std::string, PlayerUpdate> player_updates = updates.get_players();
    EXPECT_EQ(player_updates.at("test_player").get_ready(), true);
    PhaseUpdate phase_updates = updates.get_phase();
    EXPECT_EQ(phase_updates.get_phase(), PhaseType::Buying);

    EXPECT_THROW({ game.tick({Message(StartGameCommand())}, "test_player"); }, StartGameError);
}

TEST_F(TestGame, PlayerCannotJoinStartedGame) {
    game.join_player("test_player");
    Message msg_start = Message(StartGameCommand());
    game.tick({msg_start}, "test_player");

    EXPECT_THROW({ game.join_player("another_player"); }, JoinGameError);
}

TEST_F(TestGame, PlayerCannotSelectTeamWhenStartedGame) {
    auto& state = game.join_player("test_player");
    Team old_team = state.get_players().at("test_player")->get_state().get_team();

    Team new_team = Team::CT;
    if (old_team == Team::CT)
        new_team = Team::TT;

    Message msg_select_team = Message(SelectTeamCommand(new_team));
    Message msg_start = Message(StartGameCommand());
    GameUpdate updates = game.tick({msg_select_team, msg_start}, "test_player");

    msg_select_team = Message(SelectTeamCommand(old_team));
    EXPECT_THROW({ game.tick({msg_select_team}, "test_player"); }, SelectTeamError);

    if (new_team == Team::TT) {
        EXPECT_EQ(updates.get_num_tts(), 1);
    } else {
        EXPECT_EQ(updates.get_num_cts(), 1);
    }
}

TEST_F(TestGame, IncrementRoundsPlayedAfterRoundDuration) {
    auto& state = game.join_player("test_player");
    EXPECT_EQ(state.get_num_rounds(), 0);

    Message msg_start = Message(StartGameCommand());
    game.tick({msg_start}, "test_player");

    advance_secs(PhaseTimes::buying_phase_secs);
    game.tick({}, "test_player");
    advance_secs(PhaseTimes::playing_phase_secs);
    GameUpdate updates = game.tick({}, "test_player");

    EXPECT_EQ(updates.get_num_rounds(), 1);
}

TEST_F(TestGame, OneTerroristHasBombWhenGameStarted) {
    game.join_player("test_player");
    game.join_player("another_player");

    Message msg_select_team = Message(SelectTeamCommand(Team::TT));
    game.tick({msg_select_team}, "test_player");
    game.tick({msg_select_team}, "another_player");

    Message msg_start = Message(StartGameCommand());
    game.tick({msg_start}, "test_player");
    GameUpdate updates = game.tick({msg_start}, "another_player");

    std::map<std::string, PlayerUpdate> player_updates = updates.get_players();

    if (player_updates.find("test_player") != player_updates.end()) {
        InventoryUpdate inv_updates = player_updates.at("test_player").get_inventory();
        std::map<WeaponSlot, UtilityUpdate> util_updates = inv_updates.get_utilities();
        EXPECT_EQ(util_updates.at(WeaponSlot::Bomb).get_utility(), UtilityType::C4);
    } else if (player_updates.find("another_player") != player_updates.end()) {
        InventoryUpdate inv_updates = player_updates.at("another_player").get_inventory();
        std::map<WeaponSlot, UtilityUpdate> util_updates = inv_updates.get_utilities();
        EXPECT_EQ(util_updates.at(WeaponSlot::Bomb).get_utility(), UtilityType::C4);
    } else {
        FAIL();
    }
}

TEST_F(TestGame, CounterTerroristDoesNotHaveBombWhenGameStarted) {
    game.join_player("test_player");
    Message msg_select_team = Message(SelectTeamCommand(Team::CT));
    Message msg_start = Message(StartGameCommand());

    GameUpdate updates = game.tick({msg_select_team, msg_start}, "test_player");
    std::map<std::string, PlayerUpdate> player_updates = updates.get_players();

    EXPECT_FALSE(player_updates.at("test_player").has_inventory_changed());
}

TEST_F(TestGame, PlayersSwapTeamsAfterHalfOfMaxRounds) {
    game.join_player("test_player1");
    game.join_player("test_player2");

    Message msg_select_team = Message(SelectTeamCommand(Team::TT));
    game.tick({msg_select_team}, "test_player1");

    msg_select_team = Message(SelectTeamCommand(Team::CT));
    game.tick({msg_select_team}, "test_player2");

    Message msg_start = Message(StartGameCommand());
    game.tick({msg_start}, "test_player1");
    game.tick({msg_start}, "test_player2");

    GameUpdate updates;
    for (int i = 0; i < GameConfig::max_rounds / 2; i++) {
        advance_secs(PhaseTimes::buying_phase_secs);
        game.tick({}, "test_player1");
        advance_secs(PhaseTimes::playing_phase_secs);
        updates = game.tick({}, "test_player1");
        EXPECT_EQ(updates.get_phase().get_phase(), PhaseType::RoundFinished);
        EXPECT_EQ(updates.get_num_rounds(), i + 1);
        advance_secs(PhaseTimes::round_finished_phase_secs);
        game.tick({}, "test_player");
    }

    std::map<std::string, PlayerUpdate> player_updates = updates.get_players();
    EXPECT_EQ(player_updates.at("test_player1").get_team(), Team::CT);
    EXPECT_EQ(player_updates.at("test_player2").get_team(), Team::TT);
}

TEST_F(TestGame, PlayerCanMove) {
    auto& state = game.join_player("test_player");
    Vector2D old_pos = state.get_players().at("test_player")->get_pos();

    advance_secs(PhaseTimes::buying_phase_secs);
    game.tick({}, "test_player");

    Message msg_move = Message(MoveCommand(0, 1));
    GameUpdate updates = game.tick({msg_move}, "test_player");

    std::map<std::string, PlayerUpdate> player_updates = updates.get_players();
    EXPECT_TRUE(player_updates.at("test_player").get_is_moving());
    EXPECT_EQ(player_updates.at("test_player").get_move_dx(), 0);
    EXPECT_EQ(player_updates.at("test_player").get_move_dy(), 1);
    Vector2D new_pos = player_updates.at("test_player").get_pos();
    EXPECT_EQ(new_pos.get_x(), old_pos.get_x());
    EXPECT_EQ(new_pos.get_y(), old_pos.get_y() + 1.0f * static_cast<float>(map.get_tile_size()) *
                                                         GameConfig::player_speed *
                                                         (1.0f / GameConfig::tickrate));
}

TEST_F(TestGame, PlayerCanMoveInDiagonal) {
    auto& state = game.join_player("test_player");
    Vector2D old_pos = state.get_players().at("test_player")->get_pos();

    advance_secs(PhaseTimes::buying_phase_secs);
    game.tick({}, "test_player");

    Vector2D dir(1, 1);
    Message msg_move = Message(MoveCommand(1, 1));
    GameUpdate updates = game.tick({msg_move}, "test_player");

    std::map<std::string, PlayerUpdate> player_updates = updates.get_players();
    Vector2D new_pos = player_updates.at("test_player").get_pos();

    Vector2D step = dir.normalized() * static_cast<float>(map.get_tile_size()) *
                    GameConfig::player_speed * (1.0f / GameConfig::tickrate);
    EXPECT_EQ(new_pos.get_x(), old_pos.get_x() + step.get_x());
    EXPECT_EQ(new_pos.get_y(), old_pos.get_y() + step.get_y());
}

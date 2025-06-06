#include <chrono>

#include <gtest/gtest.h>

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
#include "server/player_message.h"

class TestGame: public ::testing::Test {
protected:
    std::shared_ptr<MockClock> clock;
    Map map;
    Game game;

    TestGame():
            clock(std::make_shared<MockClock>(std::chrono::steady_clock::now())),
            map(MapBuilder("./tests/server/map/map.yaml").build()),
            game("test_game", clock, std::move(map)) {}

    void advance_secs(int secs) { clock->advance(std::chrono::seconds(secs)); }
};

TEST_F(TestGame, PlayerCanJoinGame) {
    game.join_player("test_player");
    GameUpdate update = game.get_full_update();
    EXPECT_EQ(static_cast<int>(update.get_players().size()), 1);
}

TEST_F(TestGame, PlayerCannotJoinGameTwice) {
    game.join_player("test_player");
    EXPECT_THROW({ game.join_player("test_player"); }, JoinGameError);
    GameUpdate update = game.get_full_update();
    EXPECT_EQ(static_cast<int>(update.get_players().size()), 1);
}

TEST_F(TestGame, PlayerCannotJoinGameWithInvalidName) {
    EXPECT_THROW({ game.join_player(""); }, InvalidPlayerNameError);
}

TEST_F(TestGame, PlayersCanJoinGameUntilItIsFull) {
    for (int i = 1; i <= GameConfig::max_players; i++) {
        game.join_player("test_player_" + std::to_string(i));
    }
    EXPECT_THROW({ game.join_player("extra_player"); }, JoinGameError);

    GameUpdate update = game.get_full_update();
    EXPECT_EQ(static_cast<int>(update.get_players().size()), GameConfig::max_players);
}

TEST_F(TestGame, PlayerCanSelectTeam) {
    game.join_player("test_player");
    auto player_messages = game.tick({});
    GameUpdate initial_update = player_messages[0].get_message().get_content<GameUpdate>();

    Team old_team = initial_update.get_players().at("test_player").get_team();

    Team new_team = Team::CT;
    if (old_team == Team::CT)
        new_team = Team::TT;

    Message msg_select_team = Message(SelectTeamCommand(new_team));
    player_messages = game.tick({PlayerMessage("test_player", msg_select_team)});
    EXPECT_EQ(player_messages.size(), 1);

    GameUpdate update = player_messages[0].get_message().get_content<GameUpdate>();

    std::map<std::string, PlayerUpdate> player_updates = update.get_players();
    EXPECT_EQ(player_updates.at("test_player").get_team(), new_team);
}

TEST_F(TestGame, PlayerJoinFullTeam) {
    Message msg_select_team = Message(SelectTeamCommand(Team::TT));
    for (int i = 1; i <= GameConfig::max_team_players; i++) {
        game.join_player("test_player_" + std::to_string(i));
        game.tick({PlayerMessage("test_player_" + std::to_string(i), msg_select_team)});
    }
    game.join_player("extra_player");
    GameUpdate update = game.get_full_update();
    EXPECT_EQ(static_cast<int>(update.get_players().size()), GameConfig::max_team_players + 1);

    auto player_messages = game.tick({PlayerMessage("extra_player", msg_select_team)});
    GameUpdate updates;
    updates = player_messages[0].get_message().get_content<GameUpdate>();
    EXPECT_FALSE(updates.has_players_changed());
}

TEST_F(TestGame, CannotStartAnAlreadyStartedGame) {
    game.join_player("test_player");

    auto player_messages = game.tick({PlayerMessage("test_player", Message(StartGameCommand()))});
    GameUpdate updates;
    updates = player_messages[0].get_message().get_content<GameUpdate>();

    std::map<std::string, PlayerUpdate> player_updates = updates.get_players();
    EXPECT_EQ(player_updates.at("test_player").get_ready(), true);
    PhaseUpdate phase_updates = updates.get_phase();
    EXPECT_EQ(phase_updates.get_phase(), PhaseType::Buying);

    EXPECT_THROW({ game.tick({PlayerMessage("test_player", Message(StartGameCommand()))}); },
                 StartGameError);
}

TEST_F(TestGame, PlayerCannotJoinStartedGame) {
    game.join_player("test_player");
    Message msg_start = Message(StartGameCommand());
    game.tick({PlayerMessage("test_player", msg_start)});

    EXPECT_THROW({ game.join_player("another_player"); }, JoinGameError);
}

TEST_F(TestGame, PlayerCannotSelectTeamWhenStartedGame) {
    game.join_player("test_player");
    GameUpdate update = game.get_full_update();
    Team old_team = update.get_players().at("test_player").get_team();

    Team new_team;
    if (old_team == Team::CT) {
        new_team = Team::TT;
    } else {
        new_team = Team::CT;
    }

    Message msg_start = Message(StartGameCommand());
    auto player_messages = game.tick({PlayerMessage("test_player", msg_start)});
    GameUpdate updates;
    updates = player_messages[0].get_message().get_content<GameUpdate>();

    Message msg_select_team = Message(SelectTeamCommand(new_team));
    EXPECT_THROW({ game.tick({PlayerMessage("test_player", msg_select_team)}); }, SelectTeamError);
}

TEST_F(TestGame, IncrementRoundsPlayedAfterRoundDuration) {
    game.join_player("test_player");
    GameUpdate update = game.get_full_update();
    EXPECT_EQ(update.get_num_rounds(), 0);

    Message msg_start = Message(StartGameCommand());
    game.tick({PlayerMessage("test_player", msg_start)});

    advance_secs(PhaseTimes::buying_phase_secs);
    game.tick({});
    advance_secs(PhaseTimes::playing_phase_secs);
    auto player_messages = game.tick({});
    GameUpdate updates;
    updates = player_messages[0].get_message().get_content<GameUpdate>();

    EXPECT_EQ(updates.get_num_rounds(), 1);
}

TEST_F(TestGame, OneTerroristHasBombWhenGameStarted) {
    game.join_player("test_player");
    game.join_player("another_player");

    Message msg_select_team = Message(SelectTeamCommand(Team::TT));
    game.tick({PlayerMessage("test_player", msg_select_team)});
    game.tick({PlayerMessage("another_player", msg_select_team)});

    Message msg_start = Message(StartGameCommand());
    game.tick({PlayerMessage("test_player", msg_start)});
    auto player_messages = game.tick({PlayerMessage("another_player", msg_start)});
    GameUpdate updates;
    updates = player_messages[0].get_message().get_content<GameUpdate>();

    std::map<std::string, PlayerUpdate> player_updates = updates.get_players();

    if (player_updates.find("test_player") != player_updates.end()) {
        InventoryUpdate inv_updates = player_updates.at("test_player").get_inventory();
        std::map<WeaponSlot, UtilityUpdate> util_updates = inv_updates.get_utilities();
        EXPECT_EQ(util_updates.at(WeaponSlot::Bomb).get_type(), UtilityType::C4);
    } else if (player_updates.find("another_player") != player_updates.end()) {
        InventoryUpdate inv_updates = player_updates.at("another_player").get_inventory();
        std::map<WeaponSlot, UtilityUpdate> util_updates = inv_updates.get_utilities();
        EXPECT_EQ(util_updates.at(WeaponSlot::Bomb).get_type(), UtilityType::C4);
    } else {
        FAIL();
    }
}

TEST_F(TestGame, CounterTerroristDoesNotHaveBombWhenGameStarted) {
    game.join_player("test_player");
    Message msg_select_team = Message(SelectTeamCommand(Team::CT));
    Message msg_start = Message(StartGameCommand());

    auto player_messages = game.tick({PlayerMessage("test_player", msg_select_team),
                                      PlayerMessage("test_player", msg_start)});
    GameUpdate updates;
    updates = player_messages[0].get_message().get_content<GameUpdate>();
    std::map<std::string, PlayerUpdate> player_updates = updates.get_players();

    EXPECT_FALSE(player_updates.at("test_player").has_inventory_changed());
}

TEST_F(TestGame, PlayersSwapTeamsAfterHalfOfMaxRounds) {
    game.join_player("test_player1");
    game.join_player("test_player2");

    Message msg_select_team = Message(SelectTeamCommand(Team::TT));
    game.tick({PlayerMessage("test_player1", msg_select_team)});

    msg_select_team = Message(SelectTeamCommand(Team::CT));
    game.tick({PlayerMessage("test_player2", msg_select_team)});

    Message msg_start = Message(StartGameCommand());
    game.tick({PlayerMessage("test_player1", msg_start)});
    game.tick({PlayerMessage("test_player2", msg_start)});

    GameUpdate updates;
    for (int i = 0; i < GameConfig::max_rounds / 2; i++) {
        advance_secs(PhaseTimes::buying_phase_secs);
        game.tick({});
        advance_secs(PhaseTimes::playing_phase_secs);
        auto player_messages = game.tick({});
        if (!player_messages.empty()) {
            updates = player_messages[0].get_message().get_content<GameUpdate>();
        } else {
            updates = game.get_full_update();
        }
        EXPECT_EQ(updates.get_phase().get_phase(), PhaseType::RoundFinished);
        EXPECT_EQ(updates.get_num_rounds(), i + 1);
        advance_secs(PhaseTimes::round_finished_phase_secs);
        game.tick({});
    }

    std::map<std::string, PlayerUpdate> player_updates = updates.get_players();
    EXPECT_EQ(player_updates.at("test_player1").get_team(), Team::CT);
    EXPECT_EQ(player_updates.at("test_player2").get_team(), Team::TT);
}

TEST_F(TestGame, PlayerCanMove) {
    GameUpdate updates;
    std::map<std::string, PlayerUpdate> player_updates;
    game.join_player("test_player");
    updates = game.get_full_update();
    Vector2D old_pos = updates.get_players().at("test_player").get_pos();

    advance_secs(PhaseTimes::buying_phase_secs);
    game.tick({});

    // Check velocity
    Vector2D dir = Vector2D(0, 32).normalized();
    Message msg_move = Message(MoveCommand(dir));
    auto player_messages = game.tick({PlayerMessage("test_player", msg_move)});
    updates = player_messages[0].get_message().get_content<GameUpdate>();
    player_updates = updates.get_players();
    Vector2D move_vel = player_updates.at("test_player").get_velocity();
    EXPECT_EQ(move_vel, dir);

    // Check updated position
    player_updates = updates.get_players();
    Vector2D new_pos = player_updates.at("test_player").get_pos();

    Vector2D step = dir * GameConfig::player_speed * (1.0f / GameConfig::tickrate);

    EXPECT_EQ(new_pos.get_x(), old_pos.get_x() + step.get_x());
    EXPECT_EQ(new_pos.get_y(), old_pos.get_y() + step.get_y());
}

TEST_F(TestGame, PlayerCanMoveInDiagonal) {
    GameUpdate updates;
    game.join_player("test_player");
    updates = game.get_full_update();
    Vector2D old_pos = updates.get_players().at("test_player").get_pos();

    advance_secs(PhaseTimes::buying_phase_secs);
    game.tick({});

    Vector2D dir = Vector2D(32, 32).normalized();
    Message msg_move = Message(MoveCommand(dir));
    auto player_messages = game.tick({PlayerMessage("test_player", msg_move)});
    updates = player_messages[0].get_message().get_content<GameUpdate>();

    std::map<std::string, PlayerUpdate> player_updates = updates.get_players();
    Vector2D new_pos = player_updates.at("test_player").get_pos();

    Vector2D step = dir * GameConfig::player_speed * (1.0f / GameConfig::tickrate);

    EXPECT_EQ(new_pos.get_x(), old_pos.get_x() + step.get_x());
    EXPECT_EQ(new_pos.get_y(), old_pos.get_y() + step.get_y());
}

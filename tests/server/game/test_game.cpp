#include <chrono>

#include <gtest/gtest.h>

#include "common/map/map.h"
#include "common/models.h"
#include "common/physics/physics_config.h"
#include "common/updates/game_update.h"
#include "common/updates/inventory_update.h"
#include "common/updates/phase_update.h"
#include "common/updates/player_update.h"
#include "server/clock/mock_clock.h"
#include "server/errors.h"
#include "server/game/game.h"
#include "server/game/game_config.h"
#include "server/game/shop.h"
#include "server/map/map_builder.h"
#include "server/player_message.h"

class TestGame: public ::testing::Test {
protected:
    std::shared_ptr<MockClock> clock;
    Map map;
    int max_players;
    Game game;

    TestGame():
            clock(std::make_shared<MockClock>(std::chrono::steady_clock::now())),
            map(MapBuilder("./tests/server/map/map.yaml").build()),
            max_players(map.max_players),
            game("test_game", clock, std::move(map)) {}

    void advance_secs(float secs) { clock->advance(std::chrono::duration<float>(secs)); }
};

TEST_F(TestGame, PlayerCanJoinGame) {
    game.join_player("test_player");
    GameUpdate update = game.get_full_update();
    EXPECT_EQ(static_cast<int>(update.get_players().size()), 1);
}

TEST_F(TestGame, PlayerCannotJoinGameTwice) {
    game.join_player("test_player");
    GameUpdate updates = game.get_full_update();
    EXPECT_EQ(static_cast<int>(updates.get_players().size()), 1);
    game.join_player("test_player");
    updates = game.get_full_update();
    EXPECT_EQ(static_cast<int>(updates.get_players().size()), 1);
}

TEST_F(TestGame, PlayerCannotJoinGameWithEmptyName) {
    game.join_player("");
    GameUpdate updates = game.get_full_update();
    EXPECT_EQ(static_cast<int>(updates.get_players().size()), 0);
}

TEST_F(TestGame, PlayersCanJoinGameUntilItIsFull) {
    for (int i = 1; i <= max_players; i++) {
        game.join_player("test_player_" + std::to_string(i));
    }
    game.join_player("extra_player");
    GameUpdate update = game.get_full_update();
    EXPECT_EQ(static_cast<int>(update.get_players().size()), max_players);
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

TEST_F(TestGame, PlayerCannotJoinFullTeam) {
    Message msg_select_team = Message(SelectTeamCommand(Team::TT));
    int max_team_players = max_players / 2;
    for (int i = 1; i <= max_team_players; i++) {
        game.join_player("test_player_" + std::to_string(i));
        game.tick({PlayerMessage("test_player_" + std::to_string(i), msg_select_team)});
    }
    game.join_player("extra_player");
    GameUpdate update = game.get_full_update();
    EXPECT_EQ(static_cast<int>(update.get_players().size()), max_team_players + 1);

    auto player_messages = game.tick({PlayerMessage("extra_player", msg_select_team)});
    GameUpdate updates;

    updates = player_messages[0].get_message().get_content<GameUpdate>();
    EXPECT_EQ(updates.get_players().at("extra_player").get_team(), Team::CT);
}

TEST_F(TestGame, CannotStartAnAlreadyStartedGame) {
    game.join_player("test_player");
    game.tick({});

    auto player_messages = game.tick({PlayerMessage("test_player", Message(SetReadyCommand()))});
    GameUpdate updates;
    updates = player_messages[0].get_message().get_content<GameUpdate>();

    std::map<std::string, PlayerUpdate> player_updates = updates.get_players();
    EXPECT_EQ(player_updates.at("test_player").get_ready(), true);
    PhaseUpdate phase_updates = updates.get_phase();
    EXPECT_EQ(phase_updates.get_phase(), PhaseType::Buying);

    EXPECT_THROW({ game.tick({PlayerMessage("test_player", Message(SetReadyCommand()))}); },
                 SetReadyError);
}

TEST_F(TestGame, PlayerCannotJoinStartedGame) {
    game.join_player("test_player");
    Message msg_start = Message(SetReadyCommand());
    game.tick({PlayerMessage("test_player", msg_start)});

    game.join_player("another_player");
    GameUpdate update = game.get_full_update();
    EXPECT_EQ(static_cast<int>(update.get_players().size()), 1);
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

    Message msg_start = Message(SetReadyCommand());
    auto player_messages = game.tick({PlayerMessage("test_player", msg_start)});
    GameUpdate updates;
    updates = player_messages[0].get_message().get_content<GameUpdate>();

    Message msg_select_team = Message(SelectTeamCommand(new_team));
    EXPECT_THROW({ game.tick({PlayerMessage("test_player", msg_select_team)}); }, SelectTeamError);
}

TEST_F(TestGame, NumberOfRoundsIncrementCorrectly) {
    game.join_player("test_player");
    GameUpdate updates = game.get_full_update();
    EXPECT_EQ(updates.get_num_rounds(), 0);

    Message msg_start = Message(SetReadyCommand());
    game.tick({PlayerMessage("test_player", msg_start)});

    int rounds = 3;
    for (int i = 0; i < rounds; i++) {
        advance_secs(PhaseTimes::buying_phase_secs);
        game.tick({});
        advance_secs(PhaseTimes::playing_phase_secs);
        game.tick({});
        for (int j = 0; j < 10; j++) game.tick({});
        advance_secs(PhaseTimes::round_end_phase_secs);
        game.tick({});
    }

    EXPECT_EQ(game.get_full_update().get_num_rounds(), rounds);
}

TEST_F(TestGame, OneTerroristHasBombWhenGameStarted) {
    game.join_player("test_player");
    game.join_player("another_player");

    Message msg_select_team = Message(SelectTeamCommand(Team::TT));
    game.tick({PlayerMessage("test_player", msg_select_team)});
    game.tick({PlayerMessage("another_player", msg_select_team)});

    Message msg_start = Message(SetReadyCommand());
    game.tick({PlayerMessage("test_player", msg_start)});
    auto player_messages = game.tick({PlayerMessage("another_player", msg_start)});
    GameUpdate updates = player_messages[1].get_message().get_content<GameUpdate>();

    std::map<std::string, PlayerUpdate> player_updates = updates.get_players();

    EXPECT_TRUE(player_updates.at("test_player").get_inventory().has_bomb_changed() ||
                player_updates.at("another_player").get_inventory().has_bomb_changed());
}

TEST_F(TestGame, CounterTerroristDoesNotHaveBombWhenGameStarted) {
    game.join_player("test_player");
    Message msg_select_team = Message(SelectTeamCommand(Team::CT));
    Message msg_start = Message(SetReadyCommand());
    game.tick({});

    auto player_messages = game.tick({PlayerMessage("test_player", msg_select_team),
                                      PlayerMessage("test_player", msg_start)});
    GameUpdate updates;
    updates = player_messages[0].get_message().get_content<GameUpdate>();

    PlayerUpdate player_updates = updates.get_players().at("test_player");

    EXPECT_TRUE(player_updates.get_inventory().has_bomb_changed());
    EXPECT_FALSE(player_updates.get_inventory().get_bomb().has_value());
}

TEST_F(TestGame, PlayersSwapTeamsAfterHalfOfMaxRounds) {
    game.join_player("test_player1");
    game.join_player("test_player2");

    Message msg_select_team = Message(SelectTeamCommand(Team::TT));
    game.tick({PlayerMessage("test_player1", msg_select_team)});

    msg_select_team = Message(SelectTeamCommand(Team::CT));
    game.tick({PlayerMessage("test_player2", msg_select_team)});

    Message msg_start = Message(SetReadyCommand());
    game.tick({PlayerMessage("test_player1", msg_start)});
    game.tick({PlayerMessage("test_player2", msg_start)});

    GameUpdate updates;
    for (int i = 0; i < GameConfig::max_rounds / 2; i++) {
        advance_secs(PhaseTimes::buying_phase_secs);
        game.tick({});
        advance_secs(PhaseTimes::playing_phase_secs);
        game.tick({});
        EXPECT_EQ(game.get_full_update().get_phase().get_phase(), PhaseType::RoundEnd);
        advance_secs(PhaseTimes::round_end_phase_secs);
        game.tick({});
        EXPECT_EQ(game.get_full_update().get_num_rounds(), i + 1);
    }

    std::map<std::string, PlayerUpdate> player_updates = game.get_full_update().get_players();
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
    Vector2D dir = Vector2D(0, 1).normalized(PhysicsConfig::meter_size);
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

    Vector2D dir = Vector2D(1, 1).normalized(PhysicsConfig::meter_size);
    Message msg_move = Message(MoveCommand(dir));
    auto player_messages = game.tick({PlayerMessage("test_player", msg_move)});
    updates = player_messages[0].get_message().get_content<GameUpdate>();

    std::map<std::string, PlayerUpdate> player_updates = updates.get_players();
    Vector2D new_pos = player_updates.at("test_player").get_pos();

    Vector2D step = dir * GameConfig::player_speed * (1.0f / GameConfig::tickrate);

    EXPECT_EQ(new_pos.get_x(), old_pos.get_x() + step.get_x());
    EXPECT_EQ(new_pos.get_y(), old_pos.get_y() + step.get_y());
}

TEST_F(TestGame, PlayerCanAimInADirection) {
    GameUpdate updates;
    game.join_player("test_player");
    updates = game.get_full_update();

    Vector2D old_aim_dir = updates.get_players().at("test_player").get_aim_direction();
    Vector2D new_aim_dir(old_aim_dir * (-1));

    Message msg_aim = Message(AimCommand(new_aim_dir));
    auto player_messages = game.tick({PlayerMessage("test_player", msg_aim)});
    updates = player_messages[0].get_message().get_content<GameUpdate>();

    EXPECT_EQ(updates.get_players().at("test_player").get_aim_direction(), new_aim_dir);
}

TEST_F(TestGame, TargetIsHitByPlayerAttack) {
    GameUpdate updates;
    game.join_player("test_player");
    game.join_player("target_player");

    updates = game.get_full_update();
    Vector2D player_pos = updates.get_players().at("test_player").get_pos();
    Vector2D target_pos = updates.get_players().at("target_player").get_pos();
    int old_health = updates.get_players().at("target_player").get_health();

    Message msg_select_team = Message(SelectTeamCommand(Team::TT));
    game.tick({PlayerMessage("test_player", msg_select_team)});
    msg_select_team = Message(SelectTeamCommand(Team::CT));
    game.tick({PlayerMessage("target_player", msg_select_team)});

    Message msg_aim = Message(AimCommand(target_pos - player_pos));
    Message msg_switch_weap = Message(SwitchItemCommand(ItemSlot::Secondary));
    Message msg_start = Message(SetReadyCommand());
    game.tick({PlayerMessage("test_player", msg_aim), PlayerMessage("test_player", msg_switch_weap),
               PlayerMessage("test_player", msg_start), PlayerMessage("target_player", msg_start)});

    advance_secs(PhaseTimes::buying_phase_secs);

    Message msg_attack = Message(AttackCommand());
    auto player_messages = game.tick({PlayerMessage("test_player", msg_attack)});

    auto hit_response = player_messages[0].get_message().get_content<HitResponse>();
    EXPECT_EQ(hit_response.get_hit_pos(), target_pos);

    if (hit_response.is_hit()) {
        updates = player_messages[2].get_message().get_content<GameUpdate>();
        int new_health = updates.get_players().at("target_player").get_health();
        EXPECT_LT(new_health, old_health);
    }
}

TEST_F(TestGame, PlayerCanGetScoreboard) {
    game.join_player("test_player");

    Message msg_get_scoreboard = Message(GetScoreboardCommand());
    auto player_messages = game.tick({PlayerMessage("test_player", msg_get_scoreboard)});

    std::map<std::string, ScoreboardEntry> scoreboard;
    bool scoreboard_send_correctly = false;
    for (const auto& m: player_messages) {
        if (m.get_message().get_type() == MessageType::SCOREBOARD_RESP) {
            scoreboard = m.get_message().get_content<ScoreboardResponse>().get_scoreboard();
            scoreboard_send_correctly = true;
        }
    }

    EXPECT_TRUE(scoreboard_send_correctly);
    EXPECT_EQ(scoreboard.size(), 1);
    EXPECT_EQ(scoreboard.at("test_player").kills, 0);
    EXPECT_EQ(scoreboard.at("test_player").deaths, 0);
    EXPECT_EQ(scoreboard.at("test_player").score, 0);
    EXPECT_EQ(scoreboard.at("test_player").money, PlayerConfig::initial_money);
}

TEST_F(TestGame, PlayerIsDeadAfterTakingAllHealthDamage) {
    GameUpdate updates;
    game.join_player("test_player");
    game.join_player("target_player");

    updates = game.get_full_update();
    Vector2D player_pos = updates.get_players().at("test_player").get_pos();
    Vector2D target_pos = updates.get_players().at("target_player").get_pos();
    int health = updates.get_players().at("target_player").get_health();

    Message msg_select_team = Message(SelectTeamCommand(Team::TT));
    game.tick({PlayerMessage("test_player", msg_select_team)});
    msg_select_team = Message(SelectTeamCommand(Team::CT));
    game.tick({PlayerMessage("target_player", msg_select_team)});

    Message msg_aim = Message(AimCommand(target_pos - player_pos));
    Message msg_switch_weap = Message(SwitchItemCommand(ItemSlot::Secondary));
    Message msg_start = Message(SetReadyCommand());
    game.tick({PlayerMessage("test_player", msg_aim), PlayerMessage("test_player", msg_switch_weap),
               PlayerMessage("test_player", msg_start), PlayerMessage("target_player", msg_start)});

    advance_secs(PhaseTimes::buying_phase_secs);

    Message msg_attack = Message(AttackCommand());
    std::vector<PlayerMessage> player_messages;
    while (health > 0) {
        advance_secs(1.0f / GlockConfig.attack_rate);
        player_messages = game.tick({PlayerMessage("test_player", msg_attack)});
        auto hit_response = player_messages[0].get_message().get_content<HitResponse>();
        updates = player_messages[2].get_message().get_content<GameUpdate>();
        if (hit_response.is_hit())
            health = updates.get_players().at("target_player").get_health();
    }

    EXPECT_TRUE(updates.has_players_changed());
    EXPECT_EQ(health, 0);

    for (const auto& m: player_messages) {
        if (m.get_message().get_type() ==
            MessageType::SCOREBOARD_RESP) {  // cppcheck-suppress[useStlAlgorithm]
            auto scoreboard = m.get_message().get_content<ScoreboardResponse>().get_scoreboard();
            EXPECT_EQ(scoreboard.at("target_player").deaths, 1);
            EXPECT_EQ(scoreboard.at("target_player").kills, 0);
            EXPECT_EQ(scoreboard.at("test_player").score, 0);
            EXPECT_EQ(scoreboard.at("test_player").money, PlayerConfig::initial_money);

            EXPECT_EQ(scoreboard.at("test_player").kills, 1);
            EXPECT_EQ(scoreboard.at("test_player").deaths, 0);
            EXPECT_GT(scoreboard.at("test_player").score, Scores::kill);
            EXPECT_EQ(scoreboard.at("test_player").money,
                      PlayerConfig::initial_money + Bonifications::kill + Bonifications::win);
            break;
        }
    }
}

TEST_F(TestGame, WeaponDoesNotMakeDamageWhenTargetIsOutOfRange) {
    GameUpdate updates;
    game.join_player("test_player");
    game.join_player("target_player");

    updates = game.get_full_update();
    Vector2D player_pos = updates.get_players().at("test_player").get_pos();
    Vector2D target_pos = updates.get_players().at("target_player").get_pos();
    int old_health = updates.get_players().at("target_player").get_health();

    Message msg_select_team = Message(SelectTeamCommand(Team::TT));
    game.tick({PlayerMessage("test_player", msg_select_team)});
    msg_select_team = Message(SelectTeamCommand(Team::CT));
    game.tick({PlayerMessage("target_player", msg_select_team)});

    Message msg_aim = Message(AimCommand(target_pos - player_pos));
    Message msg_switch_weap = Message(SwitchItemCommand(ItemSlot::Melee));
    Message msg_start = Message(SetReadyCommand());
    game.tick({PlayerMessage("test_player", msg_aim), PlayerMessage("test_player", msg_switch_weap),
               PlayerMessage("test_player", msg_start), PlayerMessage("target_player", msg_start)});

    advance_secs(PhaseTimes::buying_phase_secs);

    Message msg_attack = Message(AttackCommand());
    game.tick({PlayerMessage("test_player", msg_attack)});

    updates = game.get_full_update();
    EXPECT_EQ(updates.get_players().at("target_player").get_health(), old_health);
}

TEST_F(TestGame, TTsWinIfTheyKillAllCTs) {
    game.join_player("tt");
    game.join_player("ct");

    Message msg_select_team = Message(SelectTeamCommand(Team::TT));
    game.tick({PlayerMessage("tt", msg_select_team)});
    msg_select_team = Message(SelectTeamCommand(Team::CT));
    game.tick({PlayerMessage("ct", msg_select_team)});

    Message msg_start = Message(SetReadyCommand());
    game.tick({PlayerMessage("tt", msg_start), PlayerMessage("ct", msg_start)});

    GameUpdate updates = game.get_full_update();
    Vector2D tt_pos = updates.get_players().at("tt").get_pos();
    Vector2D ct_pos = updates.get_players().at("ct").get_pos();

    Message msg_aim = Message(AimCommand(ct_pos - tt_pos));
    Message msg_switch_weap = Message(SwitchItemCommand(ItemSlot::Secondary));
    game.tick({PlayerMessage("tt", msg_aim), PlayerMessage("tt", msg_switch_weap)});

    advance_secs(PhaseTimes::buying_phase_secs);

    Message msg_attack = Message(AttackCommand());
    std::vector<PlayerMessage> player_messages;
    while (updates.get_players().at("ct").get_health() > 0) {
        advance_secs(1.0f / GlockConfig.attack_rate);
        int old_ct_health = updates.get_players().at("ct").get_health();
        player_messages = game.tick({PlayerMessage("tt", msg_attack)});
        updates = game.get_full_update();
        for (const auto& m: player_messages) {
            if (m.get_message().get_type() == MessageType::HIT_RESP) {
                auto hit_response = m.get_message().get_content<HitResponse>();
                if (hit_response.is_hit()) {
                    EXPECT_EQ(hit_response.get_hit_pos(), ct_pos);
                    EXPECT_LT(updates.get_players().at("ct").get_health(), old_ct_health);
                }
            }
        }
    }

    for (const auto& m: player_messages) {
        if (m.get_message().get_type() ==
            MessageType::ROUND_END_RESP) {  // cppcheck-suppress[useStlAlgorithm]
            auto round_end_resp = m.get_message().get_content<RoundEndResponse>();
            EXPECT_EQ(round_end_resp.get_winning_team(), Team::TT);
            break;
        }
    }
}

TEST_F(TestGame, PlayerStateResetCorrectlyWhenANewRoundStarts) {
    game.join_player("test_player");
    game.join_player("target_player");

    Message msg_select_team = Message(SelectTeamCommand(Team::TT));
    game.tick({PlayerMessage("test_player", msg_select_team)});
    msg_select_team = Message(SelectTeamCommand(Team::CT));
    game.tick({PlayerMessage("target_player", msg_select_team)});

    Message msg_start = Message(SetReadyCommand());
    game.tick({PlayerMessage("test_player", msg_start), PlayerMessage("target_player", msg_start)});

    GameUpdate updates = game.get_full_update();
    Vector2D tt_pos = updates.get_players().at("test_player").get_pos();
    Vector2D ct_pos = updates.get_players().at("target_player").get_pos();

    advance_secs(PhaseTimes::buying_phase_secs);

    Message msg_aim = Message(AimCommand(ct_pos - tt_pos));
    Message msg_switch_weap = Message(SwitchItemCommand(ItemSlot::Secondary));
    game.tick(
            {PlayerMessage("test_player", msg_aim), PlayerMessage("test_player", msg_switch_weap)});


    Message msg_attack = Message(AttackCommand());
    while (updates.get_players().at("target_player").get_health() == PlayerConfig::full_health) {
        advance_secs(1.0f / GlockConfig.attack_rate);
        game.tick({PlayerMessage("test_player", msg_attack)});
        updates = game.get_full_update();
    }

    Message msg_start_moving = Message(MoveCommand(Vector2D(1, 0)));
    game.tick({PlayerMessage("test_player", msg_start_moving)});

    advance_secs(PhaseTimes::playing_phase_secs);
    game.tick({});
    advance_secs(PhaseTimes::round_end_phase_secs);
    game.tick({});

    updates = game.get_full_update();
    EXPECT_EQ(updates.get_players().at("test_player").get_health(), PlayerConfig::full_health);
    EXPECT_EQ(updates.get_players().at("target_player").get_health(), PlayerConfig::full_health);
    EXPECT_EQ(updates.get_players().at("test_player").get_pos(), tt_pos);
    EXPECT_EQ(updates.get_players().at("target_player").get_pos(), ct_pos);
    EXPECT_EQ(updates.get_players().at("test_player").get_velocity(), Vector2D(0, 0));
    EXPECT_EQ(updates.get_players().at("target_player").get_velocity(), Vector2D(0, 0));
    EXPECT_EQ(updates.get_players().at("test_player").get_inventory().get_money(),
              PlayerConfig::initial_money + Bonifications::loss);
    EXPECT_EQ(updates.get_players().at("target_player").get_inventory().get_money(),
              PlayerConfig::initial_money + Bonifications::win);
}

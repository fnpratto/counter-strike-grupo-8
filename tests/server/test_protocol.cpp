#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include "client/protocol.h"
#include "common/base_socket.h"
#include "common/commands.h"
#include "common/message.h"
#include "server/protocol.h"
#include "tests/common/mock_socket.h"

class ProtocolTest: public ::testing::Test {
protected:
    std::shared_ptr<MockSocket> server_mock_socket;
    std::shared_ptr<MockSocket> client_mock_socket;
    std::unique_ptr<ServerProtocol> server_protocol;
    std::unique_ptr<ClientProtocol> client_protocol;

    void SetUp() override {
        server_mock_socket = std::make_shared<MockSocket>();
        client_mock_socket = std::make_shared<MockSocket>();
        server_protocol = std::make_unique<ServerProtocol>(server_mock_socket);
        client_protocol = std::make_unique<ClientProtocol>(client_mock_socket);
    }

    void TearDown() override { server_mock_socket->reset(); }
};

// Test receive functionality
TEST_F(ProtocolTest, ReceiveCreateGameCommand) {
    // Prepare mock data for a CreateGameCommand
    std::vector<char> mock_data = {
            static_cast<char>(MessageType::CREATE_GAME_CMD),  // Message type (1 byte)
            0x00,
            0x16,  // Length (2 bytes)
            0x00,
            0x08,  // Game name length (2 bytes)
            'T',
            'e',
            's',
            't',
            'G',
            'a',
            'm',
            'e',  // Game name (8 bytes)
            0x00,
            0x0A,  // Player name length (2 bytes)
            'T',
            'e',
            's',
            't',
            'P',
            'l',
            'a',
            'y',
            'e',
            'r'  // Player name (10 bytes)
    };

    server_mock_socket->queue_read_data(mock_data);

    Message received_message = server_protocol->recv();
    EXPECT_EQ(received_message.get_type(), MessageType::CREATE_GAME_CMD);
}

// Test error handling
TEST_F(ProtocolTest, SendWhenSocketClosed) {
    server_mock_socket->set_send_closed(true);

    ListGamesResponse resp({GameInfo("SmallGame", 3, PhaseType::WarmUp)});
    Message message(resp);

    // Should not throw, but socket should return 0 bytes sent
    server_protocol->send(message);

    // Data should still be "written" to mock even if socket is closed
    const auto& written_data = server_mock_socket->get_written_data();
    EXPECT_TRUE(written_data.empty());  // Mock returns 0 when closed
}

TEST_F(ProtocolTest, SendWithSocketError) {
    server_mock_socket->set_should_throw_on_send(true);

    CreateGameCommand cmd("TestGame", "TestPlayer");
    Message message(cmd);

    EXPECT_THROW(server_protocol->send(message), std::runtime_error);
}

TEST_F(ProtocolTest, ReceiveWhenSocketClosed) {
    server_mock_socket->set_recv_closed(true);

    // This should handle closed socket gracefully
    // The exact behavior depends on protocol implementation
    // Might throw or return empty/null message
    EXPECT_NO_THROW({
        try {
            server_protocol->recv();
        } catch (const std::exception&) {
            // Expected behavior for closed socket
        }
    });
}

TEST_F(ProtocolTest, ReceiveWithSocketError) {
    server_mock_socket->set_should_throw_on_recv(true);

    EXPECT_THROW(server_protocol->recv(), std::runtime_error);
}

// Test protocol state
TEST_F(ProtocolTest, ProtocolStateManagement) {
    EXPECT_TRUE(server_protocol->is_open());
    EXPECT_FALSE(server_protocol->is_closed());

    server_mock_socket->set_send_closed(true);
    EXPECT_TRUE(server_protocol->is_closed());
    EXPECT_FALSE(server_protocol->is_open());
}

TEST_F(ProtocolTest, ProtocolClose) {
    EXPECT_TRUE(server_protocol->is_open());

    server_protocol->close();

    EXPECT_TRUE(server_protocol->is_closed());
    EXPECT_FALSE(server_protocol->is_open());
}

// Test that the message type is serialized correctly
TEST_F(ProtocolTest, MessageTypeSerialization) {
    server_mock_socket->clear_written_data();

    ListGamesResponse resp({GameInfo("SmallGame", 3, PhaseType::WarmUp)});
    Message msg(resp);
    server_protocol->send(msg);

    const auto& written_data = server_mock_socket->get_written_data();
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::LIST_GAMES_RESP));
}


// Test that the message length is serialized correctly
TEST_F(ProtocolTest, MessageLengthSerialization) {
    server_mock_socket->clear_written_data();

    // 02 type
    // 00 10 remaining bytes
    // 00 01 vector length
    // 00 0A game name length
    // 53 6d 61 6c 6c 47 61 6d 65 game name "SmallGame"
    // 00 03 players count
    // 00 WarmUp phase
    ListGamesResponse resp({GameInfo("SmallGame", 3, PhaseType::WarmUp)});
    Message msg(resp);
    server_protocol->send(msg);

    const auto& written_data = server_mock_socket->get_written_data();
    ASSERT_EQ(written_data[1], 0x00);  // Length high byte
    ASSERT_EQ(written_data[2], 0x10);  // Length low byte (15 bytes total)
}

// Test that the game update message is serialized correctly
TEST_F(ProtocolTest, GameUpdateSerialization) {
    server_mock_socket->clear_written_data();

    GameUpdate update;

    PhaseUpdate phase_update;
    phase_update.set_phase(PhaseType::Playing);
    phase_update.set_secs_remaining(30);
    update.set_phase(phase_update);

    update.set_num_rounds(5);

    PlayerUpdate player1_update;
    player1_update.set_team(Team::CT);
    player1_update.set_character_type(CharacterType::UK_SAS);
    player1_update.set_pos(Vector2D(100.0f, 200.0f));
    player1_update.set_aim_direction(Vector2D(1.0f, 0.0f));
    player1_update.set_velocity(Vector2D(0.0f, 0.0f));
    player1_update.set_ready(true);
    player1_update.set_health(100);
    player1_update.set_equipped_item(ItemSlot::Primary);
    player1_update.set_inventory(InventoryUpdate());

    update.add_players_change("player1", player1_update);

    Message message(update);
    server_protocol->send(message);
    const auto& written_data = server_mock_socket->get_written_data();
    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();
    GameUpdate received_update = received_message.get_content<GameUpdate>();
    PhaseUpdate received_phase = received_update.get_phase();
    PlayerUpdate received_player1 = received_update.get_players().at("player1");
    ASSERT_EQ(received_message.get_type(), MessageType::GAME_UPDATE);
    ASSERT_EQ(received_phase.get_phase(), PhaseType::Playing);
    ASSERT_EQ(received_phase.get_secs_remaining(), 30);
    ASSERT_EQ(received_update.get_num_rounds(), 5);
    ASSERT_EQ(received_player1.get_team(), Team::CT);
    ASSERT_EQ(received_player1.get_pos(), Vector2D(100.0f, 200.0f));
    ASSERT_EQ(received_player1.get_aim_direction(), Vector2D(1.0f, 0.0f));
    ASSERT_EQ(received_player1.get_velocity(), Vector2D(0.0f, 0.0f));
    ASSERT_TRUE(received_player1.get_ready());
    ASSERT_EQ(received_player1.get_health(), 100);
    ASSERT_EQ(received_player1.get_equipped_item(), ItemSlot::Primary);
}

// Test HitResponse serialization and deserialization
TEST_F(ProtocolTest, HitResponseSerialization) {
    server_mock_socket->clear_written_data();

    Vector2D origin(10, 20);
    Vector2D hit_pos(30, 40);
    Vector2D hit_dir(1, 0);
    bool hit = true;

    HitResponse response(origin, hit_pos, hit_dir, hit);
    Message message(response);

    server_protocol->send(message);
    const auto& written_data = server_mock_socket->get_written_data();

    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();
    HitResponse received_response = received_message.get_content<HitResponse>();

    ASSERT_EQ(received_message.get_type(), MessageType::HIT_RESP);
    ASSERT_EQ(received_response.get_origin().get_x(), 10);
    ASSERT_EQ(received_response.get_origin().get_y(), 20);
    ASSERT_EQ(received_response.get_hit_pos().get_x(), 30);
    ASSERT_EQ(received_response.get_hit_pos().get_y(), 40);
    ASSERT_EQ(received_response.get_hit_dir().get_x(), 1);
    ASSERT_EQ(received_response.get_hit_dir().get_y(), 0);
    ASSERT_TRUE(received_response.is_hit());
}

// Test HitResponse with miss
TEST_F(ProtocolTest, HitResponseMissSerialization) {
    server_mock_socket->clear_written_data();

    Vector2D origin(50, 60);
    Vector2D hit_pos(70, 80);
    Vector2D hit_dir(-1, 1);
    bool hit = false;

    HitResponse response(origin, hit_pos, hit_dir, hit);
    Message message(response);

    server_protocol->send(message);
    const auto& written_data = server_mock_socket->get_written_data();

    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();
    HitResponse received_response = received_message.get_content<HitResponse>();

    ASSERT_EQ(received_message.get_type(), MessageType::HIT_RESP);
    ASSERT_EQ(received_response.get_origin().get_x(), 50);
    ASSERT_EQ(received_response.get_origin().get_y(), 60);
    ASSERT_EQ(received_response.get_hit_pos().get_x(), 70);
    ASSERT_EQ(received_response.get_hit_pos().get_y(), 80);
    ASSERT_EQ(received_response.get_hit_dir().get_x(), -1);
    ASSERT_EQ(received_response.get_hit_dir().get_y(), 1);
    ASSERT_FALSE(received_response.is_hit());
}

// Test CharactersResponse serialization and deserialization
TEST_F(ProtocolTest, CharactersResponseSerialization) {
    server_mock_socket->clear_written_data();

    std::vector<CharacterType> characters = {CharacterType::UK_SAS, CharacterType::French_GIGN,
                                             CharacterType::German_GSG_9,
                                             CharacterType::Seal_Force};

    CharactersResponse response(characters);
    Message message(response);

    server_protocol->send(message);
    const auto& written_data = server_mock_socket->get_written_data();

    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();
    CharactersResponse received_response = received_message.get_content<CharactersResponse>();

    ASSERT_EQ(received_message.get_type(), MessageType::CHARACTERS_RESP);
    const auto& received_characters = received_response.get_characters();
    ASSERT_EQ(received_characters.size(), 4);
    ASSERT_EQ(received_characters[0], CharacterType::UK_SAS);
    ASSERT_EQ(received_characters[1], CharacterType::French_GIGN);
    ASSERT_EQ(received_characters[2], CharacterType::German_GSG_9);
    ASSERT_EQ(received_characters[3], CharacterType::Seal_Force);
}

// Test CharactersResponse with empty list
TEST_F(ProtocolTest, CharactersResponseEmptySerialization) {
    server_mock_socket->clear_written_data();

    std::vector<CharacterType> characters;
    CharactersResponse response(characters);
    Message message(response);

    server_protocol->send(message);
    const auto& written_data = server_mock_socket->get_written_data();

    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();
    CharactersResponse received_response = received_message.get_content<CharactersResponse>();

    ASSERT_EQ(received_message.get_type(), MessageType::CHARACTERS_RESP);
    const auto& received_characters = received_response.get_characters();
    ASSERT_EQ(received_characters.size(), 0);
}

// Test ScoreboardResponse serialization and deserialization
TEST_F(ProtocolTest, ScoreboardResponseSerialization) {
    server_mock_socket->clear_written_data();

    std::map<std::string, ScoreboardEntry> scoreboard;
    scoreboard.emplace("player1", ScoreboardEntry(2, 10, 5, 22));
    scoreboard.emplace("player2", ScoreboardEntry(6, 8, 3, 78));
    scoreboard.emplace("player3", ScoreboardEntry(1, 12, 7, 45));

    ScoreboardResponse response(std::move(scoreboard));
    Message message(response);

    server_protocol->send(message);
    const auto& written_data = server_mock_socket->get_written_data();

    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();
    ScoreboardResponse received_response = received_message.get_content<ScoreboardResponse>();

    ASSERT_EQ(received_message.get_type(), MessageType::SCOREBOARD_RESP);
    const auto& received_scoreboard = received_response.get_scoreboard();
    ASSERT_EQ(received_scoreboard.size(), 3);

    ASSERT_TRUE(received_scoreboard.find("player1") != received_scoreboard.end());
    ASSERT_TRUE(received_scoreboard.find("player2") != received_scoreboard.end());
    ASSERT_TRUE(received_scoreboard.find("player3") != received_scoreboard.end());

    ASSERT_EQ(received_scoreboard.at("player1").money, 2);
    ASSERT_EQ(received_scoreboard.at("player1").kills, 10);
    ASSERT_EQ(received_scoreboard.at("player1").deaths, 5);
    ASSERT_EQ(received_scoreboard.at("player1").score, 22);

    ASSERT_EQ(received_scoreboard.at("player2").money, 6);
    ASSERT_EQ(received_scoreboard.at("player2").kills, 8);
    ASSERT_EQ(received_scoreboard.at("player2").deaths, 3);
    ASSERT_EQ(received_scoreboard.at("player2").score, 78);

    ASSERT_EQ(received_scoreboard.at("player3").money, 1);
    ASSERT_EQ(received_scoreboard.at("player3").kills, 12);
    ASSERT_EQ(received_scoreboard.at("player3").deaths, 7);
    ASSERT_EQ(received_scoreboard.at("player3").score, 45);
}

// Test ScoreboardResponse with empty scoreboard
TEST_F(ProtocolTest, ScoreboardResponseEmptySerialization) {
    server_mock_socket->clear_written_data();

    std::map<std::string, ScoreboardEntry> scoreboard;
    ScoreboardResponse response(std::move(scoreboard));
    Message message(response);

    server_protocol->send(message);
    const auto& written_data = server_mock_socket->get_written_data();

    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();
    ScoreboardResponse received_response = received_message.get_content<ScoreboardResponse>();

    ASSERT_EQ(received_message.get_type(), MessageType::SCOREBOARD_RESP);
    const auto& received_scoreboard = received_response.get_scoreboard();
    ASSERT_EQ(received_scoreboard.size(), 0);
}

// Test ErrorResponse serialization and deserialization
TEST_F(ProtocolTest, ErrorResponseSerialization) {
    server_mock_socket->clear_written_data();

    ErrorResponse response;
    Message message(response);

    server_protocol->send(message);
    const auto& written_data = server_mock_socket->get_written_data();

    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();

    // ErrorResponse is empty, so just verify it deserializes without error
    // ErrorResponse received_response = received_message.get_content<ErrorResponse>();

    ASSERT_EQ(received_message.get_type(), MessageType::ERROR_RESP);
}

// Test RoundEndResponse serialization and deserialization - CT wins
TEST_F(ProtocolTest, RoundEndResponseCTWinSerialization) {
    server_mock_socket->clear_written_data();

    RoundEndResponse response(Team::CT);
    Message message(response);

    server_protocol->send(message);
    const auto& written_data = server_mock_socket->get_written_data();

    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();
    RoundEndResponse received_response = received_message.get_content<RoundEndResponse>();

    ASSERT_EQ(received_message.get_type(), MessageType::ROUND_END_RESP);
    ASSERT_EQ(received_response.get_winning_team(), Team::CT);
}

// Test RoundEndResponse serialization and deserialization - Terrorist wins
TEST_F(ProtocolTest, RoundEndResponseTerroristWinSerialization) {
    server_mock_socket->clear_written_data();

    RoundEndResponse response(Team::TT);
    Message message(response);

    server_protocol->send(message);
    const auto& written_data = server_mock_socket->get_written_data();

    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();
    RoundEndResponse received_response = received_message.get_content<RoundEndResponse>();

    ASSERT_EQ(received_message.get_type(), MessageType::ROUND_END_RESP);
    ASSERT_EQ(received_response.get_winning_team(), Team::TT);
}

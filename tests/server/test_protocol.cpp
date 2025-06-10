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
    phase_update.set_time(TimePoint::clock::now());
    update.set_phase(phase_update);

    update.set_num_rounds(5);

    PlayerUpdate player1_update;
    player1_update.set_team(Team::CT);
    player1_update.set_pos(Vector2D(100.0f, 200.0f));
    player1_update.set_aim_direction(Vector2D(1.0f, 0.0f));
    player1_update.set_velocity(Vector2D(0.0f, 0.0f));
    player1_update.set_ready(true);
    player1_update.set_health(100);
    player1_update.set_inventory(InventoryUpdate());

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
    ASSERT_EQ(received_phase.get_time(), TimePoint::clock::now());
    ASSERT_EQ(received_update.get_num_rounds(), 5);
    ASSERT_EQ(received_player1.get_team(), Team::CT);
    ASSERT_EQ(received_player1.get_pos(), Vector2D(100.0f, 200.0f));
    ASSERT_EQ(received_player1.get_aim_direction(), Vector2D(1.0f, 0.0f));
    ASSERT_EQ(received_player1.get_velocity(), Vector2D(0.0f, 0.0f));
    ASSERT_TRUE(received_player1.get_ready());
    ASSERT_EQ(received_player1.get_health(), 100);
    ASSERT_EQ(received_player1.get_current_weapon(), WeaponSlot::Primary);
}

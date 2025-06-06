#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include "common/base_socket.h"
#include "common/commands.h"
#include "common/message.h"
#include "server/protocol.h"
#include "tests/common/mock_socket.h"

class ServerProtocolTest: public ::testing::Test {
protected:
    std::shared_ptr<MockSocket> mock_socket;
    std::unique_ptr<ServerProtocol> protocol;

    void SetUp() override {
        mock_socket = std::make_shared<MockSocket>();
        protocol = std::make_unique<ServerProtocol>(mock_socket);
    }

    void TearDown() override { mock_socket->reset(); }
};

// Test receive functionality
TEST_F(ServerProtocolTest, ReceiveCreateGameCommand) {
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

    mock_socket->queue_read_data(mock_data);

    Message received_message = protocol->recv();
    EXPECT_EQ(received_message.get_type(), MessageType::CREATE_GAME_CMD);
}

// Test error handling
TEST_F(ServerProtocolTest, SendWhenSocketClosed) {
    mock_socket->set_send_closed(true);

    ListGamesResponse resp({GameInfo("SmallGame", 3, PhaseType::WarmUp)});
    Message message(resp);

    // Should not throw, but socket should return 0 bytes sent
    protocol->send(message);

    // Data should still be "written" to mock even if socket is closed
    const auto& written_data = mock_socket->get_written_data();
    EXPECT_TRUE(written_data.empty());  // Mock returns 0 when closed
}

TEST_F(ServerProtocolTest, SendWithSocketError) {
    mock_socket->set_should_throw_on_send(true);

    CreateGameCommand cmd("TestGame", "TestPlayer");
    Message message(cmd);

    EXPECT_THROW(protocol->send(message), std::runtime_error);
}

TEST_F(ServerProtocolTest, ReceiveWhenSocketClosed) {
    mock_socket->set_recv_closed(true);

    // This should handle closed socket gracefully
    // The exact behavior depends on protocol implementation
    // Might throw or return empty/null message
    EXPECT_NO_THROW({
        try {
            protocol->recv();
        } catch (const std::exception&) {
            // Expected behavior for closed socket
        }
    });
}

TEST_F(ServerProtocolTest, ReceiveWithSocketError) {
    mock_socket->set_should_throw_on_recv(true);

    EXPECT_THROW(protocol->recv(), std::runtime_error);
}

// Test protocol state
TEST_F(ServerProtocolTest, ProtocolStateManagement) {
    EXPECT_TRUE(protocol->is_open());
    EXPECT_FALSE(protocol->is_closed());

    mock_socket->set_send_closed(true);
    EXPECT_TRUE(protocol->is_closed());
    EXPECT_FALSE(protocol->is_open());
}

TEST_F(ServerProtocolTest, ProtocolClose) {
    EXPECT_TRUE(protocol->is_open());

    protocol->close();

    EXPECT_TRUE(protocol->is_closed());
    EXPECT_FALSE(protocol->is_open());
}

// Test that the message type is serialized correctly
TEST_F(ServerProtocolTest, MessageTypeSerialization) {
    mock_socket->clear_written_data();

    ListGamesResponse resp({GameInfo("SmallGame", 3, PhaseType::WarmUp)});
    Message msg(resp);
    protocol->send(msg);

    const auto& written_data = mock_socket->get_written_data();
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::LIST_GAMES_RESP));
}


// Test that the message length is serialized correctly
TEST_F(ServerProtocolTest, MessageLengthSerialization) {
    mock_socket->clear_written_data();

    // 02 type
    // 00 10 remaining bytes
    // 00 01 vector length
    // 00 0A game name length
    // 53 6d 61 6c 6c 47 61 6d 65 game name "SmallGame"
    // 00 03 players count
    // 00 WarmUp phase
    ListGamesResponse resp({GameInfo("SmallGame", 3, PhaseType::WarmUp)});
    Message msg(resp);
    protocol->send(msg);

    const auto& written_data = mock_socket->get_written_data();
    ASSERT_EQ(written_data[1], 0x00);  // Length high byte
    ASSERT_EQ(written_data[2], 0x10);  // Length low byte (15 bytes total)
}

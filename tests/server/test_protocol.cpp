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

TEST_F(ProtocolTest, ReceiveCreateGameCommand) {
    // Test round-trip serialization/deserialization of CreateGameCommand
    client_mock_socket->clear_written_data();

    CreateGameCommand cmd("TestGame", "de_dust2", "TestPlayer");
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::CREATE_GAME_CMD));

    // Test round-trip serialization/deserialization
    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::CREATE_GAME_CMD);

    CreateGameCommand received_cmd = received_message.get_content<CreateGameCommand>();
    ASSERT_EQ(received_cmd.get_game_name(), "TestGame");
    ASSERT_EQ(received_cmd.get_map_name(), "de_dust2");
    ASSERT_EQ(received_cmd.get_player_name(), "TestPlayer");
}

TEST_F(ProtocolTest, SendReceiveJoinGameCommand) {
    client_mock_socket->clear_written_data();

    JoinGameCommand cmd("TestGame", "TestPlayer");
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::JOIN_GAME_CMD));

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::JOIN_GAME_CMD);

    JoinGameCommand received_cmd = received_message.get_content<JoinGameCommand>();
    ASSERT_EQ(received_cmd.get_game_name(), "TestGame");
    ASSERT_EQ(received_cmd.get_player_name(), "TestPlayer");
}

TEST_F(ProtocolTest, SendReceiveListGamesCommand) {
    client_mock_socket->clear_written_data();

    ListGamesCommand cmd;
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::LIST_GAMES_CMD));
    ASSERT_EQ(written_data[1], 0x00);
    ASSERT_EQ(written_data[2], 0x00);

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::LIST_GAMES_CMD);

    ListGamesCommand received_cmd = received_message.get_content<ListGamesCommand>();
}

TEST_F(ProtocolTest, SendReceiveSelectTeamCommand) {
    client_mock_socket->clear_written_data();

    SelectTeamCommand cmd(Team::CT);
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::SELECT_TEAM_CMD));

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::SELECT_TEAM_CMD);

    SelectTeamCommand received_cmd = received_message.get_content<SelectTeamCommand>();
    ASSERT_EQ(received_cmd.get_team(), Team::CT);
}

TEST_F(ProtocolTest, SendReceiveGetCharactersCommand) {
    client_mock_socket->clear_written_data();

    GetCharactersCommand cmd;
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::GET_CHARACTERS_CMD));
    ASSERT_EQ(written_data[1], 0x00);
    ASSERT_EQ(written_data[2], 0x00);

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::GET_CHARACTERS_CMD);

    GetCharactersCommand received_cmd = received_message.get_content<GetCharactersCommand>();
}

TEST_F(ProtocolTest, SendReceiveSelectCharacterCommand) {
    client_mock_socket->clear_written_data();

    SelectCharacterCommand cmd(CharacterType::UK_SAS);
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::SELECT_CHARACTER_CMD));

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::SELECT_CHARACTER_CMD);

    SelectCharacterCommand received_cmd = received_message.get_content<SelectCharacterCommand>();
    ASSERT_EQ(received_cmd.get_character_type(), CharacterType::UK_SAS);
}

// TEST_F(ProtocolTest, SendReceiveSetReadyCommand) {
//     client_mock_socket->clear_written_data();

//     SetReadyCommand cmd;
//     Message message(cmd);

//     client_protocol->send(message);

//     const auto& written_data = client_mock_socket->get_written_data();
//     ASSERT_FALSE(written_data.empty());
//     ASSERT_EQ(written_data[0], static_cast<char>(MessageType::SET_READY_CMD));
//     ASSERT_EQ(written_data[1], 0x00);
//     ASSERT_EQ(written_data[2], 0x00);

//     server_mock_socket->queue_read_data(written_data);
//     Message received_message = server_protocol->recv();
//     ASSERT_EQ(received_message.get_type(), MessageType::SET_READY_CMD);

//     SetReadyCommand received_cmd = received_message.get_content<SetReadyCommand>();
// }

TEST_F(ProtocolTest, SendReceiveGetShopPricesCommand) {
    client_mock_socket->clear_written_data();

    GetShopPricesCommand cmd;
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::GET_SHOP_PRICES_CMD));
    ASSERT_EQ(written_data[1], 0x00);
    ASSERT_EQ(written_data[2], 0x00);

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::GET_SHOP_PRICES_CMD);

    GetShopPricesCommand received_cmd = received_message.get_content<GetShopPricesCommand>();
}

TEST_F(ProtocolTest, SendReceiveBuyGunCommand) {
    client_mock_socket->clear_written_data();

    BuyGunCommand cmd(GunType::AK47);
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::BUY_GUN_CMD));

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::BUY_GUN_CMD);

    BuyGunCommand received_cmd = received_message.get_content<BuyGunCommand>();
    ASSERT_EQ(received_cmd.get_gun(), GunType::AK47);
}

TEST_F(ProtocolTest, SendReceiveBuyAmmoCommand) {
    client_mock_socket->clear_written_data();

    BuyAmmoCommand cmd(ItemSlot::Primary);
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::BUY_AMMO_CMD));

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::BUY_AMMO_CMD);

    BuyAmmoCommand received_cmd = received_message.get_content<BuyAmmoCommand>();
    ASSERT_EQ(received_cmd.get_slot(), ItemSlot::Primary);
}

TEST_F(ProtocolTest, SendReceiveMoveCommand) {
    client_mock_socket->clear_written_data();

    MoveCommand cmd(Vector2D(0, 1));
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::MOVE_CMD));

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::MOVE_CMD);

    MoveCommand received_cmd = received_message.get_content<MoveCommand>();
    ASSERT_EQ(received_cmd.get_direction(), Vector2D(0, 1));
}

TEST_F(ProtocolTest, SendReceiveStopPlayerCommand) {
    client_mock_socket->clear_written_data();

    StopPlayerCommand cmd;
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::STOP_PLAYER_CMD));
    ASSERT_EQ(written_data[1], 0x00);
    ASSERT_EQ(written_data[2], 0x00);

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::STOP_PLAYER_CMD);

    StopPlayerCommand received_cmd = received_message.get_content<StopPlayerCommand>();
}

TEST_F(ProtocolTest, SendReceiveAimCommand) {
    client_mock_socket->clear_written_data();

    AimCommand cmd(Vector2D(1.0f, 0.5f));
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::AIM_CMD));

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::AIM_CMD);

    AimCommand received_cmd = received_message.get_content<AimCommand>();
    ASSERT_EQ(received_cmd.get_direction(), Vector2D(1.0f, 0.5f));
}

TEST_F(ProtocolTest, SendReceiveAttackCommand) {
    client_mock_socket->clear_written_data();

    AttackCommand cmd;
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::ATTACK_CMD));
    ASSERT_EQ(written_data[1], 0x00);
    ASSERT_EQ(written_data[2], 0x00);

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::ATTACK_CMD);

    AttackCommand received_cmd = received_message.get_content<AttackCommand>();
}

TEST_F(ProtocolTest, SendReceiveSwitchItemCommand) {
    client_mock_socket->clear_written_data();

    SwitchItemCommand cmd(ItemSlot::Secondary);
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::SWITCH_ITEM_CMD));

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::SWITCH_ITEM_CMD);

    SwitchItemCommand received_cmd = received_message.get_content<SwitchItemCommand>();
    ASSERT_EQ(received_cmd.get_slot(), ItemSlot::Secondary);
}

TEST_F(ProtocolTest, SendReceiveReloadCommand) {
    client_mock_socket->clear_written_data();

    ReloadCommand cmd;
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::RELOAD_CMD));
    ASSERT_EQ(written_data[1], 0x00);
    ASSERT_EQ(written_data[2], 0x00);

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::RELOAD_CMD);

    ReloadCommand received_cmd = received_message.get_content<ReloadCommand>();
}

TEST_F(ProtocolTest, SendReceiveGetScoreboardCommand) {
    client_mock_socket->clear_written_data();

    GetScoreboardCommand cmd;
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::GET_SCOREBOARD_CMD));
    ASSERT_EQ(written_data[1], 0x00);
    ASSERT_EQ(written_data[2], 0x00);

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::GET_SCOREBOARD_CMD);

    GetScoreboardCommand received_cmd = received_message.get_content<GetScoreboardCommand>();
}

// TEST_F(ProtocolTest, SendReceivePlantBombCommand) {
//     client_mock_socket->clear_written_data();

//     PlantBombCommand cmd;
//     Message message(cmd);

//     client_protocol->send(message);

//     const auto& written_data = client_mock_socket->get_written_data();
//     ASSERT_FALSE(written_data.empty());
//     ASSERT_EQ(written_data[0], static_cast<char>(MessageType::PLANT_BOMB_CMD));
//     ASSERT_EQ(written_data[1], 0x00);
//     ASSERT_EQ(written_data[2], 0x00);

//     server_mock_socket->queue_read_data(written_data);
//     Message received_message = server_protocol->recv();
//     ASSERT_EQ(received_message.get_type(), MessageType::PLANT_BOMB_CMD);

//     PlantBombCommand received_cmd = received_message.get_content<PlantBombCommand>();
// }

// TEST_F(ProtocolTest, SendReceiveDefuseBombCommand) {
//     client_mock_socket->clear_written_data();

//     DefuseBombCommand cmd;
//     Message message(cmd);

//     client_protocol->send(message);

//     const auto& written_data = client_mock_socket->get_written_data();
//     ASSERT_FALSE(written_data.empty());
//     ASSERT_EQ(written_data[0], static_cast<char>(MessageType::DEFUSE_BOMB_CMD));
//     ASSERT_EQ(written_data[1], 0x00);
//     ASSERT_EQ(written_data[2], 0x00);

//     server_mock_socket->queue_read_data(written_data);
//     Message received_message = server_protocol->recv();
//     ASSERT_EQ(received_message.get_type(), MessageType::DEFUSE_BOMB_CMD);

//     DefuseBombCommand received_cmd = received_message.get_content<DefuseBombCommand>();
// }

TEST_F(ProtocolTest, SendReceivePickUpItemCommand) {
    client_mock_socket->clear_written_data();

    PickUpItemCommand cmd;
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::PICK_UP_ITEM_CMD));
    ASSERT_EQ(written_data[1], 0x00);
    ASSERT_EQ(written_data[2], 0x00);

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::PICK_UP_ITEM_CMD);

    PickUpItemCommand received_cmd = received_message.get_content<PickUpItemCommand>();
}

TEST_F(ProtocolTest, SendReceiveLeaveGameCommand) {
    client_mock_socket->clear_written_data();

    LeaveGameCommand cmd;
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::LEAVE_GAME_CMD));
    ASSERT_EQ(written_data[1], 0x00);
    ASSERT_EQ(written_data[2], 0x00);

    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::LEAVE_GAME_CMD);

    LeaveGameCommand received_cmd = received_message.get_content<LeaveGameCommand>();
}

TEST_F(ProtocolTest, ReceiveListMapsCommand) {
    // Prepare mock data for a ListMapsCommand (no payload, just message type)
    std::vector<char> mock_data = {
            static_cast<char>(MessageType::LIST_MAPS_CMD),  // Message type (1 byte)
            0x00,
            0x00,  // Length (2 bytes) - no payload
    };

    server_mock_socket->queue_read_data(mock_data);

    Message received_message = server_protocol->recv();
    EXPECT_EQ(received_message.get_type(), MessageType::LIST_MAPS_CMD);
}

// Test that the client can send a ListMapsCommand
TEST_F(ProtocolTest, SendListMapsCommand) {
    client_mock_socket->clear_written_data();

    ListMapsCommand cmd;
    Message message(cmd);

    client_protocol->send(message);

    const auto& written_data = client_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::LIST_MAPS_CMD));
    ASSERT_EQ(written_data[1], 0x00);  // Length high byte
    ASSERT_EQ(written_data[2], 0x00);  // Length low byte (0 bytes payload)

    // Test round-trip serialization/deserialization
    server_mock_socket->queue_read_data(written_data);
    Message received_message = server_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::LIST_MAPS_CMD);

    ListMapsCommand received_cmd = received_message.get_content<ListMapsCommand>();
    // ListMapsCommand has no data to verify, just verify it was deserialized correctly
}

TEST_F(ProtocolTest, SendReceiveListMapsResponse) {
    server_mock_socket->clear_written_data();

    std::vector<std::string> map_names = {"de_dust2", "de_mirage", "de_inferno"};
    ListMapsResponse resp(map_names);
    Message message(resp);

    server_protocol->send(message);

    const auto& written_data = server_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::LIST_MAPS_RESP));

    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::LIST_MAPS_RESP);

    ListMapsResponse received_resp = received_message.get_content<ListMapsResponse>();
    auto received_maps = received_resp.get_map_names();
    ASSERT_EQ(received_maps.size(), 3);
    ASSERT_EQ(received_maps[0], "de_dust2");
    ASSERT_EQ(received_maps[1], "de_mirage");
    ASSERT_EQ(received_maps[2], "de_inferno");
}

TEST_F(ProtocolTest, SendReceiveMapResponse) {
    server_mock_socket->clear_written_data();

    // Create a test map with the new interface
    Map test_map("test_map", 10, 10, 10);  // name, max_players, height, width

    // Add different types of tiles
    test_map.add_tile(Tile(Vector2D(0 * PhysicsConfig::meter_size, 0 * PhysicsConfig::meter_size),
                           1, true, false, false, false));  // collidable wall
    test_map.add_tile(Tile(Vector2D(1 * PhysicsConfig::meter_size, 1 * PhysicsConfig::meter_size),
                           2, true, false, false, false));  // collidable wall
    test_map.add_tile(Tile(Vector2D(2 * PhysicsConfig::meter_size, 2 * PhysicsConfig::meter_size),
                           3, false, true, false, false));  // terrorist spawn
    test_map.add_tile(Tile(Vector2D(3 * PhysicsConfig::meter_size, 3 * PhysicsConfig::meter_size),
                           4, false, true, false, false));  // terrorist spawn
    test_map.add_tile(Tile(Vector2D(4 * PhysicsConfig::meter_size, 4 * PhysicsConfig::meter_size),
                           5, false, false, true, false));  // counter-terrorist spawn
    test_map.add_tile(Tile(Vector2D(5 * PhysicsConfig::meter_size, 5 * PhysicsConfig::meter_size),
                           6, false, false, true, false));  // counter-terrorist spawn
    test_map.add_tile(Tile(Vector2D(6 * PhysicsConfig::meter_size, 6 * PhysicsConfig::meter_size),
                           7, false, false, false, true));  // bomb site

    Message message(test_map);

    server_protocol->send(message);

    const auto& written_data = server_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::MAP_RESP));

    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::MAP_RESP);

    Map received_map = received_message.get_content<Map>();
    ASSERT_EQ(received_map.get_name(), "test_map");
    ASSERT_EQ(received_map.get_max_players(), 10);
    ASSERT_EQ(received_map.get_height(), 10);
    ASSERT_EQ(received_map.get_width(), 10);
    ASSERT_EQ(received_map.get_collidables().size(), 2);
    ASSERT_EQ(received_map.get_spawns_tts().size(), 2);
    ASSERT_EQ(received_map.get_spawns_cts().size(), 2);
    ASSERT_EQ(received_map.get_bomb_sites().size(), 1);

    // Verify positions of specific tile types
    const auto& collidables = received_map.get_collidables();
    ASSERT_EQ(collidables[0].get().pos,
              Vector2D(0 * PhysicsConfig::meter_size, 0 * PhysicsConfig::meter_size));
    ASSERT_EQ(collidables[1].get().pos,
              Vector2D(1 * PhysicsConfig::meter_size, 1 * PhysicsConfig::meter_size));

    const auto& spawns_tts = received_map.get_spawns_tts();
    ASSERT_EQ(spawns_tts[0].get().pos,
              Vector2D(2 * PhysicsConfig::meter_size, 2 * PhysicsConfig::meter_size));
    ASSERT_EQ(spawns_tts[1].get().pos,
              Vector2D(3 * PhysicsConfig::meter_size, 3 * PhysicsConfig::meter_size));

    const auto& spawns_cts = received_map.get_spawns_cts();
    ASSERT_EQ(spawns_cts[0].get().pos,
              Vector2D(4 * PhysicsConfig::meter_size, 4 * PhysicsConfig::meter_size));
    ASSERT_EQ(spawns_cts[1].get().pos,
              Vector2D(5 * PhysicsConfig::meter_size, 5 * PhysicsConfig::meter_size));

    const auto& bomb_sites = received_map.get_bomb_sites();
    ASSERT_EQ(bomb_sites[0].get().pos,
              Vector2D(6 * PhysicsConfig::meter_size, 6 * PhysicsConfig::meter_size));
}

// Test error handling
TEST_F(ProtocolTest, SendWhenSocketClosed) {
    server_mock_socket->set_send_closed(true);

    ListGamesResponse resp({GameInfo("SmallGame", "de_dust2", 3, PhaseType::WarmUp)});
    Message message(resp);

    // Should not throw, but socket should return 0 bytes sent
    server_protocol->send(message);

    // Data should still be "written" to mock even if socket is closed
    const auto& written_data = server_mock_socket->get_written_data();
    EXPECT_TRUE(written_data.empty());  // Mock returns 0 when closed
}

TEST_F(ProtocolTest, SendWithSocketError) {
    server_mock_socket->set_should_throw_on_send(true);

    CreateGameCommand cmd("TestGame", "de_dust2", "TestPlayer");
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

    ListGamesResponse resp({GameInfo("SmallGame", "de_dust2", 3, PhaseType::WarmUp)});
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
    ListGamesResponse resp({GameInfo("SmallGame", "de_dust2", 3, PhaseType::WarmUp)});
    Message msg(resp);
    server_protocol->send(msg);

    const auto& written_data = server_mock_socket->get_written_data();
    ASSERT_EQ(written_data[1], 0x00);  // Length high byte
    ASSERT_EQ(written_data[2], 0x1A);  // Length low byte (15 bytes total)
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

// Test sending an empty ListMapsResponse
TEST_F(ProtocolTest, SendEmptyListMapsResponse) {
    server_mock_socket->clear_written_data();

    std::vector<std::string> empty_maps;
    ListMapsResponse resp(empty_maps);
    Message message(resp);

    server_protocol->send(message);

    const auto& written_data = server_mock_socket->get_written_data();
    ASSERT_FALSE(written_data.empty());
    ASSERT_EQ(written_data[0], static_cast<char>(MessageType::LIST_MAPS_RESP));

    // Test round-trip serialization/deserialization
    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();
    ASSERT_EQ(received_message.get_type(), MessageType::LIST_MAPS_RESP);

    ListMapsResponse received_resp = received_message.get_content<ListMapsResponse>();
    auto received_maps = received_resp.get_map_names();
    ASSERT_EQ(received_maps.size(), 0);
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

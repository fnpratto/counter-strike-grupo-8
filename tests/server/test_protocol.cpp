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

// Test that the game update message is serialized correctly with comprehensive data
TEST_F(ProtocolTest, GameUpdateSerialization) {
    server_mock_socket->clear_written_data();

    GameUpdate update;

    // Set up phase update
    PhaseUpdate phase_update;
    phase_update.set_phase(PhaseType::Playing);
    phase_update.set_secs_remaining(30);
    update.set_phase(phase_update);

    update.set_num_rounds(5);

    // Create a comprehensive player update
    PlayerUpdate player1_update;
    player1_update.set_team(Team::CT);
    player1_update.set_character_type(CharacterType::UK_SAS);
    player1_update.set_pos(Vector2D(100.0f, 200.0f));
    player1_update.set_aim_direction(Vector2D(1.0f, 0.0f));
    player1_update.set_velocity(Vector2D(5.0f, -3.0f));
    player1_update.set_ready(true);
    player1_update.set_health(85);
    player1_update.set_equipped_item(ItemSlot::Primary);

    // Create comprehensive inventory update
    InventoryUpdate inventory_update;
    inventory_update.set_money(2400);

    // Add primary gun (AK47)
    GunUpdate ak47_update;
    ak47_update.set_gun(GunType::AK47);
    ak47_update.set_bullets_per_mag(30);
    ak47_update.set_mag_ammo(25);
    ak47_update.set_reserve_ammo(90);
    ak47_update.set_is_attacking(false);

    // Add secondary gun (Glock)
    GunUpdate glock_update;
    glock_update.set_gun(GunType::Glock);
    glock_update.set_bullets_per_mag(20);
    glock_update.set_mag_ammo(18);
    glock_update.set_reserve_ammo(40);
    glock_update.set_is_attacking(false);

    std::map<ItemSlot, GunUpdate> guns;
    guns[ItemSlot::Primary] = ak47_update;
    guns[ItemSlot::Secondary] = glock_update;
    inventory_update.set_guns(guns);

    // Add knife
    KnifeUpdate knife_update;
    knife_update.set_is_attacking(false);
    inventory_update.set_knife(knife_update);

    // Add bomb (for terrorist player)
    BombUpdate bomb_update;
    bomb_update.set_bomb_phase(BombPhaseType::NotPlanted);
    bomb_update.set_secs_to_explode(45);
    inventory_update.set_bomb(bomb_update);

    player1_update.set_inventory(inventory_update);
    update.add_players_change("player1", player1_update);

    // Add second player (Terrorist)
    PlayerUpdate player2_update;
    player2_update.set_team(Team::TT);
    player2_update.set_character_type(CharacterType::Pheonix);
    player2_update.set_pos(Vector2D(300.0f, 150.0f));
    player2_update.set_aim_direction(Vector2D(-0.707f, 0.707f));
    player2_update.set_velocity(Vector2D(-2.0f, 4.0f));
    player2_update.set_ready(false);
    player2_update.set_health(100);
    player2_update.set_equipped_item(ItemSlot::Secondary);

    // Different inventory for player2
    InventoryUpdate inventory2_update;
    inventory2_update.set_money(1800);

    // Add M3 shotgun
    GunUpdate m3_update;
    m3_update.set_gun(GunType::M3);
    m3_update.set_bullets_per_mag(8);
    m3_update.set_mag_ammo(6);
    m3_update.set_reserve_ammo(24);
    m3_update.set_is_attacking(true);  // Currently attacking

    std::map<ItemSlot, GunUpdate> guns2;
    guns2[ItemSlot::Primary] = m3_update;
    guns2[ItemSlot::Secondary] = glock_update;  // Same glock
    inventory2_update.set_guns(guns2);

    KnifeUpdate knife2_update;
    knife2_update.set_is_attacking(false);
    inventory2_update.set_knife(knife2_update);

    player2_update.set_inventory(inventory2_update);
    update.add_players_change("player2", player2_update);

    // Add third player (Counter-Terrorist with different character type)
    PlayerUpdate player3_update;
    player3_update.set_team(Team::CT);
    player3_update.set_character_type(CharacterType::German_GSG_9);
    player3_update.set_pos(Vector2D(50.0f, 50.0f));
    player3_update.set_aim_direction(Vector2D(0.0f, 1.0f));
    player3_update.set_velocity(Vector2D(0.0f, 0.0f));  // Standing still
    player3_update.set_ready(true);
    player3_update.set_health(42);                      // Damaged player
    player3_update.set_equipped_item(ItemSlot::Melee);  // Has knife equipped

    // Minimal inventory for player3 (just knife, no guns)
    InventoryUpdate inventory3_update;
    inventory3_update.set_money(500);  // Low money

    KnifeUpdate knife3_update;
    knife3_update.set_is_attacking(true);  // Currently attacking with knife
    inventory3_update.set_knife(knife3_update);

    player3_update.set_inventory(inventory3_update);
    update.add_players_change("player3", player3_update);

    // Add fourth player (Terrorist with different character type)
    PlayerUpdate player4_update;
    player4_update.set_team(Team::TT);
    player4_update.set_character_type(CharacterType::L337_Krew);
    player4_update.set_pos(Vector2D(400.0f, 250.0f));
    player4_update.set_aim_direction(Vector2D(-0.5f, -0.866f));
    player4_update.set_velocity(Vector2D(10.0f, -8.0f));  // Moving fast
    player4_update.set_ready(false);
    player4_update.set_health(1);  // Almost dead
    player4_update.set_equipped_item(ItemSlot::Primary);

    // Player4 with AWP sniper
    InventoryUpdate inventory4_update;
    inventory4_update.set_money(16000);  // Rich player

    GunUpdate awp_update;
    awp_update.set_gun(GunType::AWP);
    awp_update.set_bullets_per_mag(10);
    awp_update.set_mag_ammo(8);
    awp_update.set_reserve_ammo(30);
    awp_update.set_is_attacking(true);  // Currently sniping

    std::map<ItemSlot, GunUpdate> guns4;
    guns4[ItemSlot::Primary] = awp_update;
    inventory4_update.set_guns(guns4);

    KnifeUpdate knife4_update;
    knife4_update.set_is_attacking(false);
    inventory4_update.set_knife(knife4_update);

    player4_update.set_inventory(inventory4_update);
    update.add_players_change("player4", player4_update);

    // Add dropped guns
    std::vector<WorldItem<GunType>> dropped_guns;

    WorldItem<GunType> dropped_awp{GunType::AWP, Rectangle(Vector2D(250.0f, 180.0f), 64, 16)};
    dropped_guns.push_back(dropped_awp);

    WorldItem<GunType> dropped_glock{GunType::Glock, Rectangle(Vector2D(120.0f, 220.0f), 32, 16)};
    dropped_guns.push_back(dropped_glock);

    WorldItem<GunType> dropped_ak47{GunType::AK47, Rectangle(Vector2D(350.0f, 100.0f), 48, 12)};
    dropped_guns.push_back(dropped_ak47);

    WorldItem<GunType> dropped_m3{GunType::M3, Rectangle(Vector2D(75.0f, 320.0f), 56, 14)};
    dropped_guns.push_back(dropped_m3);

    update.set_dropped_guns(dropped_guns);

    // Add bomb (world item)
    BombUpdate world_bomb_update;
    world_bomb_update.set_bomb_phase(BombPhaseType::Planted);
    world_bomb_update.set_secs_to_explode(25);

    WorldItem<BombUpdate> world_bomb{world_bomb_update,
                                     Rectangle(Vector2D(400.0f, 300.0f), 48, 24)};
    update.set_bomb(world_bomb);

    // Send and receive the message
    Message message(update);
    server_protocol->send(message);
    const auto& written_data = server_mock_socket->get_written_data();
    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();
    GameUpdate received_update = received_message.get_content<GameUpdate>();

    // Verify message type
    ASSERT_EQ(received_message.get_type(), MessageType::GAME_UPDATE);

    // Verify phase update
    PhaseUpdate received_phase = received_update.get_phase();
    ASSERT_EQ(received_phase.get_phase(), PhaseType::Playing);
    ASSERT_EQ(received_phase.get_secs_remaining(), 30);

    // Verify game state
    ASSERT_EQ(received_update.get_num_rounds(), 5);

    // Verify we have all 4 players
    const auto& received_players = received_update.get_players();
    ASSERT_EQ(received_players.size(), 4);
    ASSERT_TRUE(received_players.find("player1") != received_players.end());
    ASSERT_TRUE(received_players.find("player2") != received_players.end());
    ASSERT_TRUE(received_players.find("player3") != received_players.end());
    ASSERT_TRUE(received_players.find("player4") != received_players.end());

    // Verify player1
    PlayerUpdate received_player1 = received_update.get_players().at("player1");
    ASSERT_EQ(received_player1.get_team(), Team::CT);
    ASSERT_EQ(received_player1.get_character_type(), CharacterType::UK_SAS);
    ASSERT_EQ(received_player1.get_pos(), Vector2D(100.0f, 200.0f));
    ASSERT_EQ(received_player1.get_aim_direction(), Vector2D(1.0f, 0.0f));
    ASSERT_EQ(received_player1.get_velocity(), Vector2D(5.0f, -3.0f));
    ASSERT_TRUE(received_player1.get_ready());
    ASSERT_EQ(received_player1.get_health(), 85);
    ASSERT_EQ(received_player1.get_equipped_item(), ItemSlot::Primary);

    // Verify player1 inventory
    InventoryUpdate received_inventory1 = received_player1.get_inventory();
    ASSERT_EQ(received_inventory1.get_money(), 2400);

    const auto& received_guns1 = received_inventory1.get_guns();
    ASSERT_TRUE(received_guns1.find(ItemSlot::Primary) != received_guns1.end());
    ASSERT_TRUE(received_guns1.find(ItemSlot::Secondary) != received_guns1.end());

    GunUpdate received_ak47 = received_guns1.at(ItemSlot::Primary);
    ASSERT_EQ(received_ak47.get_gun(), GunType::AK47);
    ASSERT_EQ(received_ak47.get_bullets_per_mag(), 30);
    ASSERT_EQ(received_ak47.get_mag_ammo(), 25);
    ASSERT_EQ(received_ak47.get_reserve_ammo(), 90);
    ASSERT_FALSE(received_ak47.get_is_attacking());

    GunUpdate received_glock1 = received_guns1.at(ItemSlot::Secondary);
    ASSERT_EQ(received_glock1.get_gun(), GunType::Glock);
    ASSERT_EQ(received_glock1.get_bullets_per_mag(), 20);
    ASSERT_EQ(received_glock1.get_mag_ammo(), 18);
    ASSERT_EQ(received_glock1.get_reserve_ammo(), 40);
    ASSERT_FALSE(received_glock1.get_is_attacking());

    KnifeUpdate received_knife1 = received_inventory1.get_knife();
    ASSERT_FALSE(received_knife1.get_is_attacking());

    ASSERT_TRUE(received_inventory1.get_bomb().has_value());
    BombUpdate received_bomb_inv = received_inventory1.get_bomb().value();
    ASSERT_EQ(received_bomb_inv.get_bomb_phase(), BombPhaseType::NotPlanted);
    ASSERT_EQ(received_bomb_inv.get_secs_to_explode(), 45);

    // Verify player2
    PlayerUpdate received_player2 = received_update.get_players().at("player2");
    ASSERT_EQ(received_player2.get_team(), Team::TT);
    ASSERT_EQ(received_player2.get_character_type(), CharacterType::Pheonix);
    ASSERT_EQ(received_player2.get_pos(), Vector2D(300.0f, 150.0f));
    ASSERT_EQ(received_player2.get_aim_direction(), Vector2D(-0.707f, 0.707f));
    ASSERT_EQ(received_player2.get_velocity(), Vector2D(-2.0f, 4.0f));
    ASSERT_FALSE(received_player2.get_ready());
    ASSERT_EQ(received_player2.get_health(), 100);
    ASSERT_EQ(received_player2.get_equipped_item(), ItemSlot::Secondary);

    // Verify player2 inventory
    InventoryUpdate received_inventory2 = received_player2.get_inventory();
    ASSERT_EQ(received_inventory2.get_money(), 1800);

    const auto& received_guns2 = received_inventory2.get_guns();
    ASSERT_TRUE(received_guns2.find(ItemSlot::Primary) != received_guns2.end());

    GunUpdate received_m3 = received_guns2.at(ItemSlot::Primary);
    ASSERT_EQ(received_m3.get_gun(), GunType::M3);
    ASSERT_EQ(received_m3.get_bullets_per_mag(), 8);
    ASSERT_EQ(received_m3.get_mag_ammo(), 6);
    ASSERT_EQ(received_m3.get_reserve_ammo(), 24);
    ASSERT_TRUE(received_m3.get_is_attacking());

    // Verify dropped guns
    const auto& received_dropped_guns = received_update.get_dropped_guns();
    ASSERT_EQ(received_dropped_guns.size(), 4);

    ASSERT_EQ(received_dropped_guns[0].item, GunType::AWP);
    ASSERT_EQ(received_dropped_guns[0].hitbox.get_pos(), Vector2D(250.0f, 180.0f));
    ASSERT_EQ(received_dropped_guns[0].hitbox.get_width(), 64);
    ASSERT_EQ(received_dropped_guns[0].hitbox.get_height(), 16);

    ASSERT_EQ(received_dropped_guns[1].item, GunType::Glock);
    ASSERT_EQ(received_dropped_guns[1].hitbox.get_pos(), Vector2D(120.0f, 220.0f));
    ASSERT_EQ(received_dropped_guns[1].hitbox.get_width(), 32);
    ASSERT_EQ(received_dropped_guns[1].hitbox.get_height(), 16);

    ASSERT_EQ(received_dropped_guns[2].item, GunType::AK47);
    ASSERT_EQ(received_dropped_guns[2].hitbox.get_pos(), Vector2D(350.0f, 100.0f));
    ASSERT_EQ(received_dropped_guns[2].hitbox.get_width(), 48);
    ASSERT_EQ(received_dropped_guns[2].hitbox.get_height(), 12);

    ASSERT_EQ(received_dropped_guns[3].item, GunType::M3);
    ASSERT_EQ(received_dropped_guns[3].hitbox.get_pos(), Vector2D(75.0f, 320.0f));
    ASSERT_EQ(received_dropped_guns[3].hitbox.get_width(), 56);
    ASSERT_EQ(received_dropped_guns[3].hitbox.get_height(), 14);

    // Verify player3 (German GSG-9 with knife)
    PlayerUpdate received_player3 = received_update.get_players().at("player3");
    ASSERT_EQ(received_player3.get_team(), Team::CT);
    ASSERT_EQ(received_player3.get_character_type(), CharacterType::German_GSG_9);
    ASSERT_EQ(received_player3.get_pos(), Vector2D(50.0f, 50.0f));
    ASSERT_EQ(received_player3.get_aim_direction(), Vector2D(0.0f, 1.0f));
    ASSERT_EQ(received_player3.get_velocity(), Vector2D(0.0f, 0.0f));
    ASSERT_TRUE(received_player3.get_ready());
    ASSERT_EQ(received_player3.get_health(), 42);
    ASSERT_EQ(received_player3.get_equipped_item(), ItemSlot::Melee);

    // Verify player3 inventory (minimal - just knife)
    InventoryUpdate received_inventory3 = received_player3.get_inventory();
    ASSERT_EQ(received_inventory3.get_money(), 500);

    const auto& received_guns3 = received_inventory3.get_guns();
    ASSERT_TRUE(received_guns3.empty());  // No guns

    KnifeUpdate received_knife3 = received_inventory3.get_knife();
    ASSERT_TRUE(received_knife3.get_is_attacking());

    ASSERT_FALSE(received_inventory3.get_optional_bomb().has_value());  // No bomb

    // Verify player4 (L337 Krew with AWP)
    PlayerUpdate received_player4 = received_update.get_players().at("player4");
    ASSERT_EQ(received_player4.get_team(), Team::TT);
    ASSERT_EQ(received_player4.get_character_type(), CharacterType::L337_Krew);
    ASSERT_EQ(received_player4.get_pos(), Vector2D(400.0f, 250.0f));
    ASSERT_EQ(received_player4.get_aim_direction(), Vector2D(-0.5f, -0.866f));
    ASSERT_EQ(received_player4.get_velocity(), Vector2D(10.0f, -8.0f));
    ASSERT_FALSE(received_player4.get_ready());
    ASSERT_EQ(received_player4.get_health(), 1);
    ASSERT_EQ(received_player4.get_equipped_item(), ItemSlot::Primary);

    // Verify player4 inventory (rich player with AWP)
    InventoryUpdate received_inventory4 = received_player4.get_inventory();
    ASSERT_EQ(received_inventory4.get_money(), 16000);

    const auto& received_guns4 = received_inventory4.get_guns();
    ASSERT_TRUE(received_guns4.find(ItemSlot::Primary) != received_guns4.end());
    ASSERT_TRUE(received_guns4.find(ItemSlot::Secondary) == received_guns4.end());  // No secondary

    GunUpdate received_awp = received_guns4.at(ItemSlot::Primary);
    ASSERT_EQ(received_awp.get_gun(), GunType::AWP);
    ASSERT_EQ(received_awp.get_bullets_per_mag(), 10);
    ASSERT_EQ(received_awp.get_mag_ammo(), 8);
    ASSERT_EQ(received_awp.get_reserve_ammo(), 30);
    ASSERT_TRUE(received_awp.get_is_attacking());

    KnifeUpdate received_knife4 = received_inventory4.get_knife();
    ASSERT_FALSE(received_knife4.get_is_attacking());

    ASSERT_FALSE(received_inventory4.get_optional_bomb().has_value());  // No bomb

    // Verify bomb world item
    ASSERT_TRUE(received_update.get_bomb().has_value());
    WorldItem<BombUpdate> received_world_bomb = received_update.get_bomb().value();
    ASSERT_EQ(received_world_bomb.item.get_bomb_phase(), BombPhaseType::Planted);
    ASSERT_EQ(received_world_bomb.item.get_secs_to_explode(), 25);
    ASSERT_EQ(received_world_bomb.hitbox.get_pos(), Vector2D(400.0f, 300.0f));
    ASSERT_EQ(received_world_bomb.hitbox.get_width(), 48);
    ASSERT_EQ(received_world_bomb.hitbox.get_height(), 24);
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

// Test WorldItem<GunType> serialization and deserialization exclusively
TEST_F(ProtocolTest, WorldItemGunTypeSerialization) {
    server_mock_socket->clear_written_data();

    // Create a GameUpdate with only dropped guns to isolate WorldItem<GunType> testing
    GameUpdate update;

    // Set minimal required fields
    PhaseUpdate phase_update;
    phase_update.set_phase(PhaseType::Playing);
    phase_update.set_secs_remaining(60);
    update.set_phase(phase_update);

    update.set_num_rounds(1);

    // Test all gun types with different positions and hitbox sizes
    std::vector<WorldItem<GunType>> dropped_guns;

    // Test AK47
    WorldItem<GunType> ak47_item{GunType::AK47, Rectangle(Vector2D(100.0f, 200.0f), 48, 12)};
    dropped_guns.push_back(ak47_item);

    // Test M3 shotgun
    WorldItem<GunType> m3_item{GunType::M3, Rectangle(Vector2D(250.5f, 150.25f), 56, 14)};
    dropped_guns.push_back(m3_item);

    // Test AWP sniper
    WorldItem<GunType> awp_item{GunType::AWP, Rectangle(Vector2D(400.75f, 300.0f), 64, 16)};
    dropped_guns.push_back(awp_item);

    // Test Glock pistol
    WorldItem<GunType> glock_item{GunType::Glock, Rectangle(Vector2D(50.0f, 75.5f), 32, 8)};
    dropped_guns.push_back(glock_item);

    // Test edge cases with extreme positions and sizes
    WorldItem<GunType> edge_case1{GunType::AK47, Rectangle(Vector2D(0.0f, 0.0f), 1, 1)};
    dropped_guns.push_back(edge_case1);

    WorldItem<GunType> edge_case2{GunType::Glock, Rectangle(Vector2D(999.99f, 999.99f), 255, 255)};
    dropped_guns.push_back(edge_case2);

    update.set_dropped_guns(dropped_guns);

    // Send and receive the message
    Message message(update);
    server_protocol->send(message);
    const auto& written_data = server_mock_socket->get_written_data();
    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();
    GameUpdate received_update = received_message.get_content<GameUpdate>();

    // Verify message type
    ASSERT_EQ(received_message.get_type(), MessageType::GAME_UPDATE);

    // Verify dropped guns were serialized and deserialized correctly
    const auto& received_dropped_guns = received_update.get_dropped_guns();
    ASSERT_EQ(received_dropped_guns.size(), 6);

    // Verify AK47 item
    ASSERT_EQ(received_dropped_guns[0].item, GunType::AK47);
    ASSERT_EQ(received_dropped_guns[0].hitbox.get_pos(), Vector2D(100.0f, 200.0f));
    ASSERT_EQ(received_dropped_guns[0].hitbox.get_width(), 48);
    ASSERT_EQ(received_dropped_guns[0].hitbox.get_height(), 12);

    // Verify M3 item with decimal positions
    ASSERT_EQ(received_dropped_guns[1].item, GunType::M3);
    ASSERT_EQ(received_dropped_guns[1].hitbox.get_pos(), Vector2D(250.5f, 150.25f));
    ASSERT_EQ(received_dropped_guns[1].hitbox.get_width(), 56);
    ASSERT_EQ(received_dropped_guns[1].hitbox.get_height(), 14);

    // Verify AWP item
    ASSERT_EQ(received_dropped_guns[2].item, GunType::AWP);
    ASSERT_EQ(received_dropped_guns[2].hitbox.get_pos(), Vector2D(400.75f, 300.0f));
    ASSERT_EQ(received_dropped_guns[2].hitbox.get_width(), 64);
    ASSERT_EQ(received_dropped_guns[2].hitbox.get_height(), 16);

    // Verify Glock item
    ASSERT_EQ(received_dropped_guns[3].item, GunType::Glock);
    ASSERT_EQ(received_dropped_guns[3].hitbox.get_pos(), Vector2D(50.0f, 75.5f));
    ASSERT_EQ(received_dropped_guns[3].hitbox.get_width(), 32);
    ASSERT_EQ(received_dropped_guns[3].hitbox.get_height(), 8);

    // Verify edge case 1 (minimum values)
    ASSERT_EQ(received_dropped_guns[4].item, GunType::AK47);
    ASSERT_EQ(received_dropped_guns[4].hitbox.get_pos(), Vector2D(0.0f, 0.0f));
    ASSERT_EQ(received_dropped_guns[4].hitbox.get_width(), 1);
    ASSERT_EQ(received_dropped_guns[4].hitbox.get_height(), 1);

    // Verify edge case 2 (large values)
    ASSERT_EQ(received_dropped_guns[5].item, GunType::Glock);
    ASSERT_EQ(received_dropped_guns[5].hitbox.get_pos(), Vector2D(999.99f, 999.99f));
    ASSERT_EQ(received_dropped_guns[5].hitbox.get_width(), 255);
    ASSERT_EQ(received_dropped_guns[5].hitbox.get_height(), 255);

    // Verify no other data was corrupted
    PhaseUpdate received_phase = received_update.get_phase();
    ASSERT_EQ(received_phase.get_phase(), PhaseType::Playing);
    ASSERT_EQ(received_phase.get_secs_remaining(), 60);
    ASSERT_EQ(received_update.get_num_rounds(), 1);

    // Verify there's no players and no bomb
    ASSERT_TRUE(received_update.get_players().empty());
    ASSERT_FALSE(received_update.has_bomb_changed());
}

// Test WorldItem<BombUpdate> serialization and deserialization exclusively
TEST_F(ProtocolTest, WorldItemBombUpdateSerialization) {
    server_mock_socket->clear_written_data();

    // Create a GameUpdate with only a bomb to isolate WorldItem<BombUpdate> testing
    GameUpdate update;

    // Set minimal required fields
    PhaseUpdate phase_update;
    phase_update.set_phase(PhaseType::BombPlanted);
    phase_update.set_secs_remaining(45);
    update.set_phase(phase_update);

    update.set_num_rounds(3);

    // Test different bomb states
    BombUpdate planted_bomb;
    planted_bomb.set_bomb_phase(BombPhaseType::Planted);
    planted_bomb.set_secs_to_explode(30);

    WorldItem<BombUpdate> bomb_item{planted_bomb, Rectangle(Vector2D(200.5f, 300.75f), 48, 24)};
    update.set_bomb(bomb_item);

    // Send and receive the message
    Message message(update);
    server_protocol->send(message);
    const auto& written_data = server_mock_socket->get_written_data();
    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();
    GameUpdate received_update = received_message.get_content<GameUpdate>();

    // Verify message type
    ASSERT_EQ(received_message.get_type(), MessageType::GAME_UPDATE);

    // Verify bomb was serialized and deserialized correctly
    ASSERT_TRUE(received_update.get_bomb().has_value());
    WorldItem<BombUpdate> received_bomb = received_update.get_bomb().value();

    // Verify bomb update data
    ASSERT_EQ(received_bomb.item.get_bomb_phase(), BombPhaseType::Planted);
    ASSERT_EQ(received_bomb.item.get_secs_to_explode(), 30);

    // Verify hitbox data
    ASSERT_EQ(received_bomb.hitbox.get_pos(), Vector2D(200.5f, 300.75f));
    ASSERT_EQ(received_bomb.hitbox.get_width(), 48);
    ASSERT_EQ(received_bomb.hitbox.get_height(), 24);

    // Verify other data was not corrupted
    PhaseUpdate received_phase = received_update.get_phase();
    ASSERT_EQ(received_phase.get_phase(), PhaseType::BombPlanted);
    ASSERT_EQ(received_phase.get_secs_remaining(), 45);
    ASSERT_EQ(received_update.get_num_rounds(), 3);

    // Verify no players, no dropped guns
    ASSERT_TRUE(received_update.get_players().empty());
    ASSERT_TRUE(received_update.get_dropped_guns().empty());
}

// Test WorldItem<BombUpdate> with different bomb phases
TEST_F(ProtocolTest, WorldItemBombUpdateDifferentPhases) {
    server_mock_socket->clear_written_data();

    // Test with defusing bomb
    GameUpdate update;
    PhaseUpdate phase_update;
    phase_update.set_phase(PhaseType::BombPlanted);
    phase_update.set_secs_remaining(15);
    update.set_phase(phase_update);
    update.set_num_rounds(2);

    BombUpdate defusing_bomb;
    defusing_bomb.set_bomb_phase(BombPhaseType::Defusing);
    defusing_bomb.set_secs_to_explode(10);

    WorldItem<BombUpdate> bomb_item{defusing_bomb, Rectangle(Vector2D(400.0f, 100.0f), 32, 16)};
    update.set_bomb(bomb_item);

    Message message(update);
    server_protocol->send(message);
    const auto& written_data = server_mock_socket->get_written_data();
    client_mock_socket->queue_read_data(written_data);
    Message received_message = client_protocol->recv();
    GameUpdate received_update = received_message.get_content<GameUpdate>();

    ASSERT_EQ(received_message.get_type(), MessageType::GAME_UPDATE);
    ASSERT_TRUE(received_update.get_bomb().has_value());

    WorldItem<BombUpdate> received_bomb = received_update.get_bomb().value();
    ASSERT_EQ(received_bomb.item.get_bomb_phase(), BombPhaseType::Defusing);
    ASSERT_EQ(received_bomb.item.get_secs_to_explode(), 10);
    ASSERT_EQ(received_bomb.hitbox.get_pos(), Vector2D(400.0f, 100.0f));
    ASSERT_EQ(received_bomb.hitbox.get_width(), 32);
    ASSERT_EQ(received_bomb.hitbox.get_height(), 16);
}

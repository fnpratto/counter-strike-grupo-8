#include "protocol.h"

#include <algorithm>
#include <atomic>
#include <iostream>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "common/commands.h"
#include "common/message.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"
#include "game/game.h"

#include "errors.h"
#include "lobby_monitor.h"

// === Serialization ===

template <>
payload_t ServerProtocol::serialize_msg(const ListGamesResponse& response) const {
    auto game_names = response.get_game_names();
    return serialize(game_names);
}

payload_t ServerProtocol::serialize_message(const Message& message) const {
    switch (message.get_type()) {
        case MessageType::LIST_GAMES_RESP: {
            const auto& response = message.get_content<ListGamesResponse>();
            return serialize_msg(response);
        }
        default:
            throw std::runtime_error("Invalid message type for serialization");
    }
}

// === Deserialization ===


template <>
CreateGameCommand ServerProtocol::deserialize_msg<CreateGameCommand>(payload_t& payload) const {
    std::string game_name = deserialize<std::string>(payload);
    std::string player_name = deserialize<std::string>(payload);
    return CreateGameCommand(game_name, player_name);
}

template <>
ListGamesCommand ServerProtocol::deserialize_msg<ListGamesCommand>(payload_t& payload) const {
    (void)payload;
    return ListGamesCommand();
}

template <>
JoinGameCommand ServerProtocol::deserialize_msg<JoinGameCommand>(payload_t& payload) const {
    std::string game_name = deserialize<std::string>(payload);
    std::string player_name = deserialize<std::string>(payload);
    return JoinGameCommand(game_name, player_name);
}

template <>
SelectTeamCommand ServerProtocol::deserialize_msg<SelectTeamCommand>(payload_t& payload) const {
    uint8_t team = deserialize<uint8_t>(payload);
    return SelectTeamCommand(static_cast<Team>(team));
}

template <>
StartGameCommand ServerProtocol::deserialize_msg<StartGameCommand>(payload_t& payload) const {
    (void)payload;
    return StartGameCommand();
}

template <>
BuyGunCommand ServerProtocol::deserialize_msg<BuyGunCommand>(payload_t& payload) const {
    uint8_t gun = deserialize<uint8_t>(payload);
    return BuyGunCommand(static_cast<GunType>(gun));
}

template <>
MoveCommand ServerProtocol::deserialize_msg<MoveCommand>(payload_t& payload) const {
    Vector2D dir = deserialize<Vector2D>(payload);
    return MoveCommand(dir);
}

template <>
StopPlayerCommand ServerProtocol::deserialize_msg<StopPlayerCommand>(payload_t& payload) const {
    (void)payload;
    return StopPlayerCommand();
}

template <>
AimCommand ServerProtocol::deserialize_msg<AimCommand>(payload_t& payload) const {
    float x = deserialize<float>(payload);
    float y = deserialize<float>(payload);
    return AimCommand(x, y);
}

template <>
ShootCommand ServerProtocol::deserialize_msg<ShootCommand>(payload_t& payload) const {
    (void)payload;
    return ShootCommand();
}

template <>
ReloadCommand ServerProtocol::deserialize_msg<ReloadCommand>(payload_t& payload) const {
    (void)payload;
    return ReloadCommand();
}

template <>
SwitchWeaponCommand ServerProtocol::deserialize_msg<SwitchWeaponCommand>(payload_t& payload) const {
    uint8_t slot = deserialize<uint8_t>(payload);
    return SwitchWeaponCommand(static_cast<WeaponSlot>(slot));
}

template <>
PlantBombCommand ServerProtocol::deserialize_msg<PlantBombCommand>(payload_t& payload) const {
    (void)payload;
    return PlantBombCommand();
}

template <>
DefuseBombCommand ServerProtocol::deserialize_msg<DefuseBombCommand>(payload_t& payload) const {
    (void)payload;
    return DefuseBombCommand();
}

template <>
PickUpItemCommand ServerProtocol::deserialize_msg<PickUpItemCommand>(payload_t& payload) const {
    (void)payload;
    return PickUpItemCommand();
}

template <>
LeaveGameCommand ServerProtocol::deserialize_msg<LeaveGameCommand>(payload_t& payload) const {
    (void)payload;
    return LeaveGameCommand();
}

Message ServerProtocol::deserialize_message(const MessageType& msg_type, payload_t& payload) const {
    switch (msg_type) {
        case MessageType::CREATE_GAME_CMD:
            return Message(deserialize_msg<CreateGameCommand>(payload));
        case MessageType::LIST_GAMES_CMD:
            return Message(deserialize_msg<ListGamesCommand>(payload));
        case MessageType::JOIN_GAME_CMD:
            return Message(deserialize_msg<JoinGameCommand>(payload));
        case MessageType::SELECT_TEAM_CMD:
            return Message(deserialize_msg<SelectTeamCommand>(payload));
        case MessageType::START_GAME_CMD:
            return Message(deserialize_msg<StartGameCommand>(payload));
        case MessageType::BUY_GUN_CMD:
            return Message(deserialize_msg<BuyGunCommand>(payload));
        case MessageType::MOVE_CMD:
            return Message(deserialize_msg<MoveCommand>(payload));
        case MessageType::STOP_PLAYER_CMD:
            return Message(deserialize_msg<StopPlayerCommand>(payload));
        case MessageType::AIM_CMD:
            return Message(deserialize_msg<AimCommand>(payload));
        case MessageType::SHOOT_CMD:
            return Message(deserialize_msg<ShootCommand>(payload));
        case MessageType::RELOAD_CMD:
            return Message(deserialize_msg<ReloadCommand>(payload));
        case MessageType::SWITCH_WEAPON_CMD:
            return Message(deserialize_msg<SwitchWeaponCommand>(payload));
        case MessageType::PLANT_BOMB_CMD:
            return Message(deserialize_msg<PlantBombCommand>(payload));
        case MessageType::DEFUSE_BOMB_CMD:
            return Message(deserialize_msg<DefuseBombCommand>(payload));
        case MessageType::PICK_UP_ITEM_CMD:
            return Message(deserialize_msg<PickUpItemCommand>(payload));
        case MessageType::LEAVE_GAME_CMD:
            return Message(deserialize_msg<LeaveGameCommand>(payload));

        default:
            throw std::runtime_error("Invalid command received");
    }
}

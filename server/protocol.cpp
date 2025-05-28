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

#include "common/deserializers.h"
#include "common/message.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"

#include "errors.h"
#include "game.h"
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

DESERIALIZERS(ServerProtocol)

template <>
CreateGameCommand ServerProtocol::deserialize<CreateGameCommand>(payload_t& payload) const {
    return CreateGameCommand(deserialize<std::string>(payload));
}

template <>
ListGamesCommand ServerProtocol::deserialize<ListGamesCommand>(payload_t& payload) const {
    (void)payload;
    return ListGamesCommand();
}

template <>
JoinGameCommand ServerProtocol::deserialize<JoinGameCommand>(payload_t& payload) const {
    return JoinGameCommand(deserialize<std::string>(payload));
}

template <>
AimCommand ServerProtocol::deserialize<AimCommand>(payload_t& payload) const {
    float x = deserialize<float>(payload);
    float y = deserialize<float>(payload);
    return AimCommand(x, y);
}

template <>
SelectTeamCommand ServerProtocol::deserialize<SelectTeamCommand>(payload_t& payload) const {
    uint8_t team = deserialize<uint8_t>(payload);
    return SelectTeamCommand(static_cast<Team>(team));
}

template <>
BuyWeaponCommand ServerProtocol::deserialize<BuyWeaponCommand>(payload_t& payload) const {
    uint8_t weapon = deserialize<uint8_t>(payload);
    return BuyWeaponCommand(static_cast<WeaponType>(weapon));
}

template <>
MoveCommand ServerProtocol::deserialize<MoveCommand>(payload_t& payload) const {
    uint8_t dir = deserialize<uint8_t>(payload);
    return MoveCommand(static_cast<MoveDirection>(dir));
}

template <>
SwitchWeaponCommand ServerProtocol::deserialize<SwitchWeaponCommand>(payload_t& payload) const {
    uint8_t slot = deserialize<uint8_t>(payload);
    return SwitchWeaponCommand(static_cast<WeaponSlot>(slot));
}

Message ServerProtocol::deserialize_message(const MessageType& msg_type, payload_t& payload) const {
    switch (msg_type) {
        case MessageType::CREATE_GAME_CMD:
            return Message(deserialize<CreateGameCommand>(payload));
        case MessageType::LIST_GAMES_CMD:
            return Message(deserialize<ListGamesCommand>(payload));
        case MessageType::JOIN_GAME_CMD:
            return Message(deserialize<JoinGameCommand>(payload));
        case MessageType::AIM_CMD:
            return Message(deserialize<AimCommand>(payload));
        case MessageType::SELECT_TEAM_CMD:
            return Message(deserialize<SelectTeamCommand>(payload));
        case MessageType::BUY_WEAPON_CMD:
            return Message(deserialize<BuyWeaponCommand>(payload));
        case MessageType::MOVE_CMD:
            return Message(deserialize<MoveCommand>(payload));
        case MessageType::SWITCH_WEAPON_CMD:
            return Message(deserialize<SwitchWeaponCommand>(payload));

        default:
            throw std::runtime_error("Invalid command received");
    }
}

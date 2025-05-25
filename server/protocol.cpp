#include "common/protocol.h"

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

#include "common/message.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"

#include "errors.h"
#include "game.h"
#include "lobby_monitor.h"
#include "protocol.h"

// === Serialization ===

template <>
payload_t ServerProtocol::serialize(const CreateGameCommand& value) {
    payload_t payload;
    std::string game_name = value.get_game_name();
    uint16_t name_length = htons(static_cast<uint16_t>(game_name.length()));
    payload.insert(payload.end(), reinterpret_cast<const char*>(&name_length),
                   reinterpret_cast<const char*>(&name_length) + sizeof(name_length));
    payload.insert(payload.end(), game_name.begin(), game_name.end());
    return payload;
}

template <>
payload_t ServerProtocol::serialize(const JoinGameCommand& value) {
    payload_t payload;
    std::string game_name = value.get_game_name();
    uint16_t name_length = htons(static_cast<uint16_t>(game_name.length()));
    payload.insert(payload.end(), reinterpret_cast<const char*>(&name_length),
                   reinterpret_cast<const char*>(&name_length) + sizeof(name_length));
    payload.insert(payload.end(), game_name.begin(), game_name.end());
    return payload;
}

template <>
payload_t ServerProtocol::serialize(const ListGamesCommand& value) {
    (void)value;
    return payload_t();
}

payload_t ServerProtocol::serialize_message(const Message& message) {
    switch (message.get_type()) {
        case MessageType::CREATE_GAME_CMD:
            return serialize(message.get_content<CreateGameCommand>());
        case MessageType::JOIN_GAME_CMD:
            return serialize(message.get_content<JoinGameCommand>());
        case MessageType::LIST_GAMES_CMD:
            return serialize(message.get_content<ListGamesCommand>());
        default:
            throw std::runtime_error("Invalid message type for serialization");
    }
}

// === Deserialization ===

template <>
CreateGameCommand ServerProtocol::deserialize<CreateGameCommand>(const payload_t& payload) {
    // Read 2 bytes for string length
    uint16_t name_length = *reinterpret_cast<const uint16_t*>(payload.data());
    name_length = ntohs(name_length);
    // Read string
    std::string game_name(payload.data() + sizeof(name_length), name_length);
    return CreateGameCommand(game_name);
}

template <>
JoinGameCommand ServerProtocol::deserialize<JoinGameCommand>(const payload_t& payload) {
    // Read 2 bytes for string length
    uint16_t name_length = *reinterpret_cast<const uint16_t*>(payload.data() + sizeof(uint16_t));
    name_length = ntohs(name_length);
    std::string game_name(payload.data() + sizeof(name_length), name_length);

    return JoinGameCommand(game_name);
}

template <>
ListGamesCommand ServerProtocol::deserialize<ListGamesCommand>(const payload_t& payload) {
    (void)payload;
    return ListGamesCommand();
}

Message ServerProtocol::deserialize_message(const MessageType& msg_type, const payload_t& payload) {
    switch (msg_type) {
        case MessageType::CREATE_GAME_CMD:
            return Message(deserialize<CreateGameCommand>(payload));

        case MessageType::JOIN_GAME_CMD:
            return Message(deserialize<JoinGameCommand>(payload));

        case MessageType::LIST_GAMES_CMD:
            return Message(deserialize<ListGamesCommand>(payload));

        default:
            throw std::runtime_error("Invalid command received");
    }
}

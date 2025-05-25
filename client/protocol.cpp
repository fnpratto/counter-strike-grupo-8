#include "common/protocol.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "common/message.h"
#include "common/socket.h"

#include "errors.h"
#include "protocol.h"

// === Serialization ===

template <>
payload_t ClientProtocol::serialize(const CreateGameCommand& cmd) {
    payload_t payload;

    uint8_t cmd_type = static_cast<uint8_t>(CmdType::CMD_CREATE);
    std::string game_name = cmd.get_game_name();
    uint16_t name_length = htons(static_cast<uint16_t>(game_name.length()));

    payload.reserve(sizeof(cmd_type) + sizeof(name_length) + game_name.length());

    payload.insert(payload.end(), &cmd_type, &cmd_type + sizeof(cmd_type));
    payload.insert(payload.end(), reinterpret_cast<const char*>(&name_length),
                   reinterpret_cast<const char*>(&name_length) + sizeof(name_length));
    payload.insert(payload.end(), game_name.begin(), game_name.end());

    return payload;
}

template <>
payload_t ClientProtocol::serialize(const JoinGameCommand& cmd) {
    payload_t payload;

    uint8_t cmd_type = static_cast<uint8_t>(CmdType::CMD_JOIN);
    std::string game_name = cmd.get_game_name();
    uint16_t name_length = htons(static_cast<uint16_t>(game_name.length()));

    payload.reserve(sizeof(cmd_type) + sizeof(name_length) + game_name.length());

    payload.insert(payload.end(), &cmd_type, &cmd_type + sizeof(cmd_type));
    payload.insert(payload.end(), reinterpret_cast<const char*>(&name_length),
                   reinterpret_cast<const char*>(&name_length) + sizeof(name_length));
    payload.insert(payload.end(), game_name.begin(), game_name.end());

    return payload;
}

template <>
payload_t ClientProtocol::serialize(const ListGamesCommand& cmd) {
    (void)cmd;
    payload_t payload;

    uint8_t cmd_type = static_cast<uint8_t>(CmdType::CMD_LIST);
    payload.reserve(sizeof(cmd_type));
    payload.insert(payload.end(), &cmd_type, &cmd_type + sizeof(cmd_type));

    return payload;
}

payload_t ClientProtocol::serialize_message(const Message& message) {
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
CreateGameCommand ClientProtocol::deserialize<CreateGameCommand>(const payload_t& payload) {
    if (payload.size() < 2) {
        throw std::runtime_error("Invalid payload size");
    }

    uint16_t name_length = ntohs(*reinterpret_cast<const uint16_t*>(payload.data()));
    if (payload.size() < 2 + name_length) {
        throw std::runtime_error("Invalid payload size");
    }

    std::string game_name(payload.data() + 2, payload.data() + 2 + name_length);
    return CreateGameCommand(game_name);
}

template <>
JoinGameCommand ClientProtocol::deserialize<JoinGameCommand>(const payload_t& payload) {
    if (payload.size() < 2) {
        throw std::runtime_error("Invalid payload size");
    }

    uint16_t name_length = ntohs(*reinterpret_cast<const uint16_t*>(payload.data()));
    if (payload.size() < 2 + name_length) {
        throw std::runtime_error("Invalid payload size");
    }

    std::string game_name(payload.data() + 2, payload.data() + 2 + name_length);
    return JoinGameCommand(game_name);
}

template <>
ListGamesCommand ClientProtocol::deserialize<ListGamesCommand>(const payload_t& payload) {
    (void)payload;
    return ListGamesCommand();
}

Message ClientProtocol::deserialize_message(const MessageType& type, const payload_t& payload) {
    switch (type) {
        case MessageType::CREATE_GAME_CMD:
            return Message(deserialize<CreateGameCommand>(payload));
        case MessageType::JOIN_GAME_CMD:
            return Message(deserialize<JoinGameCommand>(payload));
        case MessageType::LIST_GAMES_CMD:
            return Message(deserialize<ListGamesCommand>(payload));
        default:
            throw std::runtime_error("Invalid message type for deserialization");
    }
}

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
#include "lobby_monitor.h"
#include "protocol.h"

Message ServerProtocol::recv() {
    uint8_t command;
    int bytes_read = socket.recvall(&command, 1);
    if (bytes_read <= 0)
        throw ServerDisconnectError();

    switch (static_cast<CmdType>(command)) {
        case CmdType::CMD_CREATE:
            return deserialize_create_game_cmd();

        case CmdType::CMD_JOIN:
            return deserialize_join_game_cmd();

        case CmdType::CMD_LIST:
            return deserialize_list_games_cmd();

        default:
            throw std::runtime_error("Invalid command received");
    }
}

Message ServerProtocol::deserialize_create_game_cmd() {
    uint16_t name_length_n;
    if (socket.recvall(&name_length_n, sizeof(name_length_n)) <= 0) {
        throw std::runtime_error("Connection closed while reading name length");
    }

    uint16_t name_length = ntohs(name_length_n);

    std::vector<char> name_buffer(name_length + 1, '\0');
    if (socket.recvall(name_buffer.data(), name_length) <= 0) {
        throw std::runtime_error("Connection closed while reading game name");
    }

    std::string game_name(name_buffer.data(), name_length);

    return Message(CreateGameCommand(game_name));
}

Message ServerProtocol::deserialize_join_game_cmd() {
    uint16_t name_length_n;
    if (socket.recvall(&name_length_n, sizeof(name_length_n)) <= 0)
        throw std::runtime_error("Connection closed while reading name length");

    uint16_t name_length = ntohs(name_length_n);

    // Read game name
    std::vector<char> name_buffer(name_length + 1, '\0');
    if (socket.recvall(name_buffer.data(), name_length) <= 0)
        throw std::runtime_error("Connection closed while reading game name");

    std::string game_name(name_buffer.data(), name_length);

    return Message(JoinGameCommand(game_name));
}

Message ServerProtocol::deserialize_list_games_cmd() { return Message(ListGamesCommand()); }


payload_t ServerProtocol::serialize_message(const Message& message) {
    if (message.get_type() == MessageType::NONE)
        throw std::runtime_error("Invalid message type for serialization");

    // payload_t base_payload;

    // switch (message.get_type()) {
    //     case MessageType::GAME_STATUS:
    //         base_payload = serialize_game_status(message.get_content<GameStatus>());
    //         break;
    //     case MessageType::GAMES_LIST: {
    //         auto names = message.get_content<std::vector<std::string>>();
    //         base_payload = serialize_game_names(names);
    //         break;
    //     }
    //     default:
    //         throw std::runtime_error("Invalid message type for serialization");
    // }

    payload_t payload;

    // uint16_t length = htons(static_cast<uint16_t>(base_payload.size()));
    // payload.reserve(sizeof(length) + base_payload.size());

    // payload.insert(payload.end(), reinterpret_cast<const char*>(&length),
    //                reinterpret_cast<const char*>(&length) + sizeof(length));

    // payload.insert(payload.end(), base_payload.begin(), base_payload.end());

    return payload;
}

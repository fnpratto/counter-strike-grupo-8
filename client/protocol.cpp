#include "common/protocol.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "common/message.h"

#include "errors.h"
#include "protocol.h"
#include "socket.h"

Message ClientProtocol::recv() {
    // uint16_t length_n;
    // if (socket->recvall(&length_n, sizeof(length_n)) <= 0)
    //     throw ServerDisconnectError();
    // uint16_t length = ntohs(length_n);

    // std::vector<char> buffer(length + 1, '\0');
    // if (socket->recvall(buffer.data(), length) <= 0)
    //     throw ServerDisconnectError();

    // std::string message(buffer.data(), length);

    return Message();
}

payload_t ClientProtocol::serialize_message(const Message& message) {
    switch (message.get_type()) {
        case MessageType::CREATE_GAME_CMD:
            return serialize_create_game_cmd(message.get_content<CreateGameCommand>());
        case MessageType::JOIN_GAME_CMD:
            return serialize_join_game_cmd(message.get_content<JoinGameCommand>());
        case MessageType::LIST_GAMES_CMD:
            return serialize_list_games_cmd(message.get_content<ListGamesCommand>());
        default:
            throw std::runtime_error("Invalid message type for serialization");
    }
}

payload_t ClientProtocol::serialize_create_game_cmd(const CreateGameCommand& cmd) {
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

payload_t ClientProtocol::serialize_join_game_cmd(const JoinGameCommand& cmd) {
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

payload_t ClientProtocol::serialize_list_games_cmd(const ListGamesCommand& cmd) {
    payload_t payload;

    uint8_t cmd_type = static_cast<uint8_t>(CmdType::CMD_LIST);
    payload.reserve(sizeof(cmd_type));
    payload.insert(payload.end(), &cmd_type, &cmd_type + sizeof(cmd_type));

    return payload;
}

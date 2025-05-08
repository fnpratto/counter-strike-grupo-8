#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "common/message.h"
#include "common/protocol.h"

#include "errors.h"
#include "socket.h"

/**
 * @class ClientProtocol
 * @brief Protocol for client-side message transmission and reception.
 */
class ClientProtocol: public BaseProtocol {
public:
    /**
     * @brief Constructs a ClientProtocol with a connected socket.
     * @param skt Shared pointer to a connected Socket.
     */
    explicit ClientProtocol(std::shared_ptr<Socket>&& skt): BaseProtocol(std::move(skt)) {}

    /**
     * @brief Receives a Message from the server.
     * @return Deserialized Message object.
     */
    Message recv() override;

    /**
     * @brief Serializes a Message for transmission.
     * @param message The Message to serialize.
     * @return Payload bytes to send.
     */
    payload_t serialize_message(const Message& message) override;

private:
    payload_t serialize_create_game_cmd(const CreateGameCommand& cmd);
    payload_t serialize_join_game_cmd(const JoinGameCommand& cmd);
    payload_t serialize_list_games_cmd(const ListGamesCommand& cmd);
};

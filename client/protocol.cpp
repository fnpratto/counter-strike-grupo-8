#include "common/protocol.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "common/deserializers.h"
#include "common/message.h"
#include "common/serializers.h"
#include "common/socket.h"

#include "errors.h"
#include "protocol.h"

// === Serialization ===

SERIALIZERS(ClientProtocol)

template <>
payload_t ClientProtocol::serialize(const CreateGameCommand& cmd) const {
    return serialize(cmd.get_game_name());
}

template <>
payload_t ClientProtocol::serialize(const JoinGameCommand& cmd) const {
    return serialize(cmd.get_game_name());
}

template <>
payload_t ClientProtocol::serialize(const ListGamesCommand& cmd) const {
    (void)cmd;
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize(const AimCommand& cmd) const {
    payload_t payload;
    payload.reserve(2 * sizeof(float));

    payload_t x = serialize(cmd.get_x());
    payload_t y = serialize(cmd.get_y());

    payload.reserve(x.size() + y.size());
    payload.insert(payload.end(), x.begin(), x.end());
    payload.insert(payload.end(), y.begin(), y.end());

    return payload;
}

template <>
payload_t ClientProtocol::serialize(const SelectTeamCommand& cmd) const {
    return serialize(static_cast<uint8_t>(cmd.get_team()));
}

template <>
payload_t ClientProtocol::serialize(const BuyWeaponCommand& cmd) const {
    return serialize(static_cast<uint8_t>(cmd.get_weapon()));
}

template <>
payload_t ClientProtocol::serialize(const MoveCommand& cmd) const {
    return serialize(static_cast<uint8_t>(cmd.get_direction()));
}

template <>
payload_t ClientProtocol::serialize(const SwitchWeaponCommand& cmd) const {
    return serialize(static_cast<uint8_t>(cmd.get_slot()));
}

payload_t ClientProtocol::serialize_message(const Message& message) const {
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

DESERIALIZERS(ClientProtocol)

Message ClientProtocol::deserialize_message(const MessageType& type, payload_t& payload) const {
    (void)payload;  // TODO remove me

    switch (type) {
        default:
            throw std::runtime_error("Invalid message type for deserialization");
    }
}

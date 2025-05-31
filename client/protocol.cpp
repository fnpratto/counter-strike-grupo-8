#include "common/protocol.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "common/message.h"
#include "common/responses.h"
#include "common/socket.h"

#include "errors.h"
#include "protocol.h"

// === Serialization ===

template <>
payload_t ClientProtocol::serialize_msg(const CreateGameCommand& cmd) const {
    return serialize(cmd.get_game_name());
}

template <>
payload_t ClientProtocol::serialize_msg(const JoinGameCommand& cmd) const {
    return serialize(cmd.get_game_name());
}

template <>
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const ListGamesCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg(const SelectTeamCommand& cmd) const {
    return serialize(static_cast<uint8_t>(cmd.get_team()));
}

template <>
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const StartGameCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg(const BuyGunCommand& cmd) const {
    return serialize(static_cast<uint8_t>(cmd.get_gun()));
}

template <>
payload_t ClientProtocol::serialize_msg(const MoveCommand& cmd) const {
    return serialize(cmd.get_direction());
}

template <>
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const StopPlayerCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg(const AimCommand& cmd) const {
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
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const ShootCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const ReloadCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg(const SwitchWeaponCommand& cmd) const {
    return serialize(static_cast<uint8_t>(cmd.get_slot()));
}

template <>
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const PlantBombCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const DefuseBombCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const PickUpItemCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const LeaveGameCommand& cmd) const {
    return payload_t();
}

payload_t ClientProtocol::serialize_message(const Message& message) const {
    switch (message.get_type()) {
        case MessageType::CREATE_GAME_CMD:
            return serialize_msg(message.get_content<CreateGameCommand>());
        case MessageType::JOIN_GAME_CMD:
            return serialize_msg(message.get_content<JoinGameCommand>());
        case MessageType::LIST_GAMES_CMD:
            return serialize_msg(message.get_content<ListGamesCommand>());
        case MessageType::SELECT_TEAM_CMD:
            return serialize_msg(message.get_content<SelectTeamCommand>());
        case MessageType::START_GAME_CMD:
            return serialize_msg(message.get_content<StartGameCommand>());
        case MessageType::BUY_GUN_CMD:
            return serialize_msg(message.get_content<BuyGunCommand>());
        case MessageType::MOVE_CMD:
            return serialize_msg(message.get_content<MoveCommand>());
        case MessageType::STOP_PLAYER_CMD:
            return serialize_msg(message.get_content<StopPlayerCommand>());
        case MessageType::AIM_CMD:
            return serialize_msg(message.get_content<AimCommand>());
        case MessageType::SHOOT_CMD:
            return serialize_msg(message.get_content<ShootCommand>());
        case MessageType::RELOAD_CMD:
            return serialize_msg(message.get_content<ReloadCommand>());
        case MessageType::SWITCH_WEAPON_CMD:
            return serialize_msg(message.get_content<SwitchWeaponCommand>());
        case MessageType::PLANT_BOMB_CMD:
            return serialize_msg(message.get_content<PlantBombCommand>());
        case MessageType::DEFUSE_BOMB_CMD:
            return serialize_msg(message.get_content<DefuseBombCommand>());
        case MessageType::PICK_UP_ITEM_CMD:
            return serialize_msg(message.get_content<PickUpItemCommand>());
        case MessageType::LEAVE_GAME_CMD:
            return serialize_msg(message.get_content<LeaveGameCommand>());
        default:
            throw std::runtime_error("Invalid message type for serialization");
    }
}

// === Deserialization ===

template <>
ListGamesResponse ClientProtocol::deserialize_msg<ListGamesResponse>(payload_t& payload) const {
    return ListGamesResponse(deserialize_vector<std::string>(payload));
}

Message ClientProtocol::deserialize_message(const MessageType& type, payload_t& payload) const {
    switch (type) {
        case MessageType::LIST_GAMES_RESP: {
            return Message(deserialize_msg<ListGamesResponse>(payload));
        }
        default:
            throw std::runtime_error("Invalid message type for deserialization");
    }
}

#include "common/protocol.h"

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "common/errors.h"
#include "common/game/scoreboard_entry.h"
#include "common/message.h"
#include "common/responses.h"
#include "common/socket.h"
#include "common/updates/bomb_update.h"
#include "common/updates/game_update.h"
#include "common/updates/gun_update.h"
#include "common/updates/inventory_update.h"
#include "common/updates/knife_update.h"
#include "common/updates/phase_update.h"
#include "common/updates/player_update.h"

#include "protocol.h"

#define ATTR(...) __VA_ARGS__

// === Serialization ===

template <>
payload_t ClientProtocol::serialize_msg(const CreateGameCommand& cmd) const {
    payload_t payload;
    payload_t game_name = serialize(cmd.get_game_name());
    payload_t player_name = serialize(cmd.get_player_name());

    payload.reserve(game_name.size() + player_name.size());
    payload.insert(payload.end(), game_name.begin(), game_name.end());
    payload.insert(payload.end(), player_name.begin(), player_name.end());

    return payload;
}

template <>
payload_t ClientProtocol::serialize_msg(const JoinGameCommand& cmd) const {
    payload_t payload;
    payload_t game_name = serialize(cmd.get_game_name());
    payload_t player_name = serialize(cmd.get_player_name());

    payload.reserve(game_name.size() + player_name.size());
    payload.insert(payload.end(), game_name.begin(), game_name.end());
    payload.insert(payload.end(), player_name.begin(), player_name.end());

    return payload;
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
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const GetCharactersCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg(const SelectCharacterCommand& cmd) const {
    return serialize(static_cast<uint8_t>(cmd.get_character_type()));
}

template <>
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const SetReadyCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const GetShopPricesCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg(const BuyGunCommand& cmd) const {
    return serialize(static_cast<uint8_t>(cmd.get_gun()));
}

template <>

payload_t ClientProtocol::serialize_msg(const BuyAmmoCommand& cmd) const {
    return serialize(cmd.get_slot());
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

    payload_t x = serialize(cmd.get_direction().get_x());
    payload_t y = serialize(cmd.get_direction().get_y());

    payload.reserve(x.size() + y.size());
    payload.insert(payload.end(), x.begin(), x.end());
    payload.insert(payload.end(), y.begin(), y.end());

    return payload;
}

template <>
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const AttackCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg(const SwitchItemCommand& cmd) const {
    return serialize(static_cast<uint8_t>(cmd.get_slot()));
}

template <>
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const ReloadCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const GetScoreboardCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg(
        [[maybe_unused]] const StartPlantingBombCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const StopPlantingBombCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg(
        [[maybe_unused]] const StartDefusingBombCommand& cmd) const {
    return payload_t();
}

template <>
payload_t ClientProtocol::serialize_msg([[maybe_unused]] const StopDefusingBombCommand& cmd) const {
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

#define SERIALIZE_MSG(command, msg_type) \
    case MessageType::msg_type:          \
        return serialize_msg(message.get_content<command>());

payload_t ClientProtocol::serialize_message(const Message& message) const {
    switch (message.get_type()) {
        LOBBY_COMMANDS_MAP(SERIALIZE_MSG)
        GAME_COMMANDS_MAP(SERIALIZE_MSG)
        default:
            throw std::runtime_error("Invalid message type for serialization");
    }
}

#undef SERIALIZE_MSG

// === Deserialization ===

template <>
ListGamesResponse ClientProtocol::deserialize_msg<ListGamesResponse>(payload_t& payload) const {
    return ListGamesResponse(deserialize<std::vector<GameInfo>>(payload));
}

template <>
ShopPricesResponse ClientProtocol::deserialize_msg<ShopPricesResponse>(payload_t& payload) const {
    auto gun_prices = deserialize<GunType, int>(payload);
    auto ammo_prices = deserialize<GunType, int>(payload);

    return ShopPricesResponse(gun_prices, ammo_prices);
}

template <>
HitResponse ClientProtocol::deserialize_msg<HitResponse>(payload_t& payload) const {

    // Deserialize origin Vector2D
    int origin_x = deserialize<int>(payload);
    int origin_y = deserialize<int>(payload);
    Vector2D origin(origin_x, origin_y);

    // Deserialize hit_pos Vector2D
    int hit_pos_x = deserialize<int>(payload);
    int hit_pos_y = deserialize<int>(payload);
    Vector2D hit_pos(hit_pos_x, hit_pos_y);

    // Deserialize hit_dir Vector2D
    int hit_dir_x = deserialize<int>(payload);
    int hit_dir_y = deserialize<int>(payload);
    Vector2D hit_dir(hit_dir_x, hit_dir_y);

    // Deserialize hit bool
    bool hit = deserialize<bool>(payload);

    return HitResponse(origin, hit_pos, hit_dir, hit);
}

template <>
CharactersResponse ClientProtocol::deserialize_msg<CharactersResponse>(payload_t& payload) const {
    return CharactersResponse(deserialize<std::vector<CharacterType>>(payload));
}

template <>
ScoreboardResponse ClientProtocol::deserialize_msg<ScoreboardResponse>(payload_t& payload) const {
    auto scoreboard = deserialize<std::string, ScoreboardEntry>(payload);
    return ScoreboardResponse(std::move(scoreboard));
}

template <>
ErrorResponse ClientProtocol::deserialize_msg<ErrorResponse>(
        [[maybe_unused]] payload_t& payload) const {
    return ErrorResponse();
}

template <>
RoundEndResponse ClientProtocol::deserialize_msg<RoundEndResponse>(payload_t& payload) const {
    uint8_t team = deserialize<uint8_t>(payload);
    return RoundEndResponse(static_cast<Team>(team));
}

template <>
BombPlantedResponse ClientProtocol::deserialize_msg<BombPlantedResponse>(
        [[maybe_unused]] payload_t& payload) const {
    return BombPlantedResponse();
}

template <>
BombExplodedResponse ClientProtocol::deserialize_msg<BombExplodedResponse>(
        [[maybe_unused]] payload_t& payload) const {
    Vector2D explosion_center = deserialize<Vector2D>(payload);
    int explosion_radius = deserialize<int>(payload);
    return BombExplodedResponse(explosion_center, explosion_radius);
}

template <>
BombDefusedResponse ClientProtocol::deserialize_msg<BombDefusedResponse>(
        [[maybe_unused]] payload_t& payload) const {
    return BombDefusedResponse();
}

template <>
GameUpdate ClientProtocol::deserialize_msg<GameUpdate>(payload_t& payload) const {
    return deserialize<GameUpdate>(payload);
}

#define DESERIALIZE_MSG(msg, msg_type) \
    case MessageType::msg_type:        \
        return Message(deserialize_msg<msg>(payload));

Message ClientProtocol::deserialize_message(const MessageType& type, payload_t& payload) const {
    switch (type) {
        RESPONSES_MAP(DESERIALIZE_MSG)
        UPDATES_MAP(DESERIALIZE_MSG)
        default:
            throw std::runtime_error("Invalid message type for deserialization");
    }
}

#undef DESERIALIZE_MSG

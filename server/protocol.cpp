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
#include "common/updates/game_update.h"
#include "game/game.h"

#include "errors.h"
#include "lobby_monitor.h"

// === Serialization ===

template <>
payload_t ServerProtocol::serialize_msg(const ListGamesResponse& response) const {
    return serialize(response.get_games_info());
}

template <>
payload_t ServerProtocol::serialize_msg(const ListMapsResponse& response) const {
    return serialize(response.get_map_names());
}

template <>
payload_t ServerProtocol::serialize_msg(const ShopPricesResponse& response) const {
    payload_t payload;
    payload_t gun_prices_payload = serialize(response.get_gun_prices());
    payload_t ammo_prices_payload = serialize(response.get_ammo_prices());

    payload.reserve(gun_prices_payload.size() + ammo_prices_payload.size());
    payload.insert(payload.end(), gun_prices_payload.begin(), gun_prices_payload.end());
    payload.insert(payload.end(), ammo_prices_payload.begin(), ammo_prices_payload.end());

    return payload;
}

template <>
payload_t ServerProtocol::serialize_msg(const HitResponse& response) const {
    payload_t payload;
    // Serialize origin Vector2D
    payload_t origin_x = serialize(response.get_origin().get_x());
    payload_t origin_y = serialize(response.get_origin().get_y());

    // Serialize hit_pos Vector2D
    payload_t hit_pos_x = serialize(response.get_hit_pos().get_x());
    payload_t hit_pos_y = serialize(response.get_hit_pos().get_y());

    // Serialize hit_dir Vector2D
    payload_t hit_dir_x = serialize(response.get_hit_dir().get_x());
    payload_t hit_dir_y = serialize(response.get_hit_dir().get_y());

    // Serialize hit bool
    payload_t hit = serialize(response.is_hit());

    // Reserve space and insert all data
    payload.reserve(origin_x.size() + origin_y.size() + hit_pos_x.size() + hit_pos_y.size() +
                    hit_dir_x.size() + hit_dir_y.size() + hit.size());

    payload.insert(payload.end(), origin_x.begin(), origin_x.end());
    payload.insert(payload.end(), origin_y.begin(), origin_y.end());
    payload.insert(payload.end(), hit_pos_x.begin(), hit_pos_x.end());
    payload.insert(payload.end(), hit_pos_y.begin(), hit_pos_y.end());
    payload.insert(payload.end(), hit_dir_x.begin(), hit_dir_x.end());
    payload.insert(payload.end(), hit_dir_y.begin(), hit_dir_y.end());
    payload.insert(payload.end(), hit.begin(), hit.end());

    return payload;
}

template <>
payload_t ServerProtocol::serialize_msg(const CharactersResponse& response) const {
    return serialize(response.get_characters());
}

template <>
payload_t ServerProtocol::serialize_msg([[maybe_unused]] const ScoreboardResponse& response) const {
    payload_t payload;
    payload_t scoreboard_payload = serialize(response.get_scoreboard());
    payload.reserve(scoreboard_payload.size());
    payload.insert(payload.end(), scoreboard_payload.begin(), scoreboard_payload.end());
    return payload;
}

template <>
payload_t ServerProtocol::serialize_msg([[maybe_unused]] const ErrorResponse& response) const {
    return payload_t();
}

template <>
payload_t ServerProtocol::serialize_msg(const RoundEndResponse& response) const {
    return serialize(static_cast<uint8_t>(response.get_winning_team()));
}

template <>
payload_t ServerProtocol::serialize_msg(const Map& map) const {
    return serialize(map);
}

template <>
payload_t ServerProtocol::serialize_msg(const BombExplodedResponse& response) const {
    payload_t payload;
    payload_t explosion_center = serialize(response.get_explosion_center());
    payload_t explosion_radius = serialize(response.get_explosion_radius());

    payload.reserve(explosion_center.size() + explosion_radius.size());
    payload.insert(payload.end(), explosion_center.begin(), explosion_center.end());
    payload.insert(payload.end(), explosion_radius.begin(), explosion_radius.end());

    return payload;
}

template <>
payload_t ServerProtocol::serialize_msg([[maybe_unused]] const SwapTeamsResponse& response) const {
    return payload_t();
}

#define SERIALIZE_MSG(msg, msg_type) \
    case MessageType::msg_type:      \
        return serialize_msg(message.get_content<msg>());

payload_t ServerProtocol::serialize_message(const Message& message) const {
    switch (message.get_type()) {
        RESPONSES_MAP(SERIALIZE_MSG)

        case MessageType::GAME_UPDATE:
            return serialize(message.get_content<GameUpdate>());
        default:
            throw std::runtime_error("Invalid message type for serialization");
    }
}

#undef SERIALIZE_MSG

// === Deserialization ===

template <>
CreateGameCommand ServerProtocol::deserialize_msg<CreateGameCommand>(payload_t& payload) const {
    std::string game_name = deserialize<std::string>(payload);
    std::string map_name = deserialize<std::string>(payload);
    std::string player_name = deserialize<std::string>(payload);
    return CreateGameCommand(game_name, map_name, player_name);
}

template <>
ListGamesCommand ServerProtocol::deserialize_msg<ListGamesCommand>(payload_t& payload) const {
    (void)payload;
    return ListGamesCommand();
}

template <>
ListMapsCommand ServerProtocol::deserialize_msg<ListMapsCommand>(payload_t& payload) const {
    (void)payload;
    return ListMapsCommand();
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
GetCharactersCommand ServerProtocol::deserialize_msg<GetCharactersCommand>(
        payload_t& payload) const {
    (void)payload;
    return GetCharactersCommand();
}

template <>
SelectCharacterCommand ServerProtocol::deserialize_msg<SelectCharacterCommand>(
        payload_t& payload) const {
    uint8_t character_type = deserialize<uint8_t>(payload);
    return SelectCharacterCommand(static_cast<CharacterType>(character_type));
}

template <>
SetReadyCommand ServerProtocol::deserialize_msg<SetReadyCommand>(payload_t& payload) const {
    (void)payload;
    return SetReadyCommand();
}

template <>
GetShopPricesCommand ServerProtocol::deserialize_msg<GetShopPricesCommand>(
        payload_t& payload) const {
    (void)payload;
    return GetShopPricesCommand();
}

template <>
BuyGunCommand ServerProtocol::deserialize_msg<BuyGunCommand>(payload_t& payload) const {
    uint8_t gun = deserialize<uint8_t>(payload);
    return BuyGunCommand(static_cast<GunType>(gun));
}

template <>
BuyAmmoCommand ServerProtocol::deserialize_msg<BuyAmmoCommand>(payload_t& payload) const {
    ItemSlot slot = deserialize<ItemSlot>(payload);
    return BuyAmmoCommand(slot);
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
    int x = deserialize<int>(payload);
    int y = deserialize<int>(payload);
    return AimCommand(Vector2D(x, y));
}

template <>
AttackCommand ServerProtocol::deserialize_msg<AttackCommand>(payload_t& payload) const {
    (void)payload;
    return AttackCommand();
}

template <>
SwitchItemCommand ServerProtocol::deserialize_msg<SwitchItemCommand>(payload_t& payload) const {
    uint8_t slot = deserialize<uint8_t>(payload);
    return SwitchItemCommand(static_cast<ItemSlot>(slot));
}

template <>
ReloadCommand ServerProtocol::deserialize_msg<ReloadCommand>(payload_t& payload) const {
    (void)payload;
    return ReloadCommand();
}

template <>
GetScoreboardCommand ServerProtocol::deserialize_msg<GetScoreboardCommand>(
        payload_t& payload) const {
    (void)payload;
    return GetScoreboardCommand();
}

template <>
StartPlantingBombCommand ServerProtocol::deserialize_msg<StartPlantingBombCommand>(
        payload_t& payload) const {
    (void)payload;
    return StartPlantingBombCommand();
}

template <>
StopPlantingBombCommand ServerProtocol::deserialize_msg<StopPlantingBombCommand>(
        payload_t& payload) const {
    (void)payload;
    return StopPlantingBombCommand();
}

template <>
StartDefusingBombCommand ServerProtocol::deserialize_msg<StartDefusingBombCommand>(
        payload_t& payload) const {
    (void)payload;
    return StartDefusingBombCommand();
}

template <>
StopDefusingBombCommand ServerProtocol::deserialize_msg<StopDefusingBombCommand>(
        payload_t& payload) const {
    (void)payload;
    return StopDefusingBombCommand();
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

#define DESERIALIZE_MSG(command, msg_type) \
    case MessageType::msg_type:            \
        return Message(deserialize_msg<command>(payload));

Message ServerProtocol::deserialize_message(const MessageType& msg_type, payload_t& payload) const {
    switch (msg_type) {
        LOBBY_COMMANDS_MAP(DESERIALIZE_MSG)
        GAME_COMMANDS_MAP(DESERIALIZE_MSG)
        default:
            throw std::runtime_error("Invalid command received");
    }
}

#undef DESERIALIZE_MSG

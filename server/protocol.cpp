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
payload_t ServerProtocol::serialize_msg(const ShopPricesResponse& response) const {
    payload_t payload;
    payload_t gun_prices_payload = serialize_map(response.get_gun_prices());
    payload_t ammo_prices_payload = serialize_map(response.get_ammo_prices());

    payload.reserve(gun_prices_payload.size() + ammo_prices_payload.size());
    payload.insert(payload.end(), gun_prices_payload.begin(), gun_prices_payload.end());
    payload.insert(payload.end(), ammo_prices_payload.begin(), ammo_prices_payload.end());

    return payload;
}

template <>
payload_t ServerProtocol::serialize_msg([[maybe_unused]] const HitResponse& response) const {
    return payload_t();
}

template <>
payload_t ServerProtocol::serialize_msg(const CharactersResponse& response) const {
    return serialize(response.get_characters());
}

template <>
payload_t ServerProtocol::serialize_msg([[maybe_unused]] const ScoreboardResponse& response) const {
    return payload_t();
}

template <>
payload_t ServerProtocol::serialize_msg(
        [[maybe_unused]] const TriedToJoinFullTeamErrorResponse& response) const {
    return payload_t();
}

template <>
payload_t ServerProtocol::serialize_msg(
        [[maybe_unused]] const CannotBuyErrorResponse& response) const {
    return payload_t();
}

#define X_SERIALIZE_UPDATE(type, attr)                                               \
    payload.push_back(update.has_##attr##_changed());                                \
    if (update.has_##attr##_changed()) {                                             \
        payload_t attr##_payload = serialize(update.get_##attr());                   \
        payload.insert(payload.end(), attr##_payload.begin(), attr##_payload.end()); \
    }
#define M_SERIALIZE_UPDATE(key_type, value_type, attr)                               \
    payload.push_back(update.has_##attr##_changed());                                \
    if (update.has_##attr##_changed()) {                                             \
        payload_t attr##_payload = serialize_map(update.get_##attr());               \
        payload.insert(payload.end(), attr##_payload.begin(), attr##_payload.end()); \
    }
#define U_SERIALIZE_UPDATE(type, attr)                                               \
    payload.push_back(update.has_##attr##_changed());                                \
    if (update.has_##attr##_changed()) {                                             \
        payload_t attr##_payload = serialize_update(update.get_##attr());            \
        payload.insert(payload.end(), attr##_payload.begin(), attr##_payload.end()); \
    }
#define O_SERIALIZE_UPDATE(type, attr)                                               \
    payload.push_back(update.has_##attr##_changed());                                \
    if (update.has_##attr##_changed()) {                                             \
        payload_t attr##_payload = serialize_optional(update.get_##attr());          \
        payload.insert(payload.end(), attr##_payload.begin(), attr##_payload.end()); \
    }

#define SERIALIZE_UPDATE(CLASS, ATTRS)                                                        \
    template <>                                                                               \
    payload_t ServerProtocol::serialize_update(const CLASS& update) const {                   \
        payload_t payload;                                                                    \
                                                                                              \
        ATTRS(X_SERIALIZE_UPDATE, M_SERIALIZE_UPDATE, U_SERIALIZE_UPDATE, O_SERIALIZE_UPDATE) \
                                                                                              \
        return payload;                                                                       \
    }

SERIALIZE_UPDATE(BombUpdate, BOMB_ATTRS)
SERIALIZE_UPDATE(KnifeUpdate, KNIFE_ATTRS)
SERIALIZE_UPDATE(GunUpdate, GUN_ATTRS)
SERIALIZE_UPDATE(InventoryUpdate, INVENTORY_ATTRS)
SERIALIZE_UPDATE(PlayerUpdate, PLAYER_ATTRS)
SERIALIZE_UPDATE(PhaseUpdate, PHASE_ATTRS)
SERIALIZE_UPDATE(GameUpdate, GAME_ATTRS)

#define SERIALIZE_MSG(msg, msg_type) \
    case MessageType::msg_type:      \
        return serialize_msg(message.get_content<msg>());

payload_t ServerProtocol::serialize_message(const Message& message) const {
    switch (message.get_type()) {
        RESPONSES_MAP(SERIALIZE_MSG)
        case MessageType::GAME_UPDATE:
            return serialize_update(message.get_content<GameUpdate>());
        default:
            throw std::runtime_error("Invalid message type for serialization");
    }
}

#undef SERIALIZE_MSG

// === Deserialization ===

template <>
CreateGameCommand ServerProtocol::deserialize_msg<CreateGameCommand>(payload_t& payload) const {
    std::string game_name = deserialize<std::string>(payload);
    std::string player_name = deserialize<std::string>(payload);
    return CreateGameCommand(game_name, player_name);
}

template <>
ListGamesCommand ServerProtocol::deserialize_msg<ListGamesCommand>(payload_t& payload) const {
    (void)payload;
    return ListGamesCommand();
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

// TODO: Implement
template <>
BuyAmmoCommand ServerProtocol::deserialize_msg<BuyAmmoCommand>(
        [[maybe_unused]] payload_t& payload) const {
    return BuyAmmoCommand(ItemSlot::Primary);
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
PlantBombCommand ServerProtocol::deserialize_msg<PlantBombCommand>(payload_t& payload) const {
    (void)payload;
    return PlantBombCommand();
}

template <>
DefuseBombCommand ServerProtocol::deserialize_msg<DefuseBombCommand>(payload_t& payload) const {
    (void)payload;
    return DefuseBombCommand();
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

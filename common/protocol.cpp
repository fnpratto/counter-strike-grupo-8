#include "protocol.h"

#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "common/game/world_item.h"
#include "common/map/map.h"
#include "common/map/tile.h"
#include "common/models.h"
#include "common/socket.h"
#include "common/updates/bomb_update.h"
#include "common/utils/vector_2d.h"
#include "server/clock/clock.h"

#include "message.h"

BaseProtocol::BaseProtocol(std::shared_ptr<BaseSocket> skt): socket(std::move(skt)) {
    if (!socket) {
        throw std::runtime_error("BaseProtocol: null socket provided");
    }
}

bool BaseProtocol::is_closed() const {
    return socket->is_stream_recv_closed() || socket->is_stream_send_closed();
}

bool BaseProtocol::is_open() const { return !is_closed(); }


template <>
payload_t BaseProtocol::serialize(const uint8_t& i) const {
    payload_t payload(1);
    payload[0] = static_cast<char>(i);
    return payload;
}

template <>
payload_t BaseProtocol::serialize(const int16_t& i) const {
    payload_t payload;

    int16_t data = htons(static_cast<int16_t>(i));
    payload.insert(payload.begin(), reinterpret_cast<const char*>(&data),
                   reinterpret_cast<const char*>(&data) + sizeof(data));

    return payload;
}

template <>
payload_t BaseProtocol::serialize(const int& i) const {
    return serialize(static_cast<int16_t>(i));
}

template <>
payload_t BaseProtocol::serialize(const bool& b) const {
    payload_t payload(1);
    payload[0] = static_cast<char>(b);
    return payload;
}

template <>
payload_t BaseProtocol::serialize(const float& f) const {
    payload_t payload;

    float network_f = htonl(f);
    payload.insert(payload.end(), reinterpret_cast<const char*>(&network_f),
                   reinterpret_cast<const char*>(&network_f) + sizeof(network_f));

    return payload;
}

payload_t BaseProtocol::serialize(MessageType type) const {
    payload_t header(1);
    header[0] = static_cast<uint8_t>(type);
    return header;
}

template <>
payload_t BaseProtocol::serialize(const uint16_t& i) const {
    payload_t payload;

    uint16_t data = htons(static_cast<uint16_t>(i));
    payload.insert(payload.begin(), reinterpret_cast<const char*>(&data),
                   reinterpret_cast<const char*>(&data) + sizeof(data));

    return payload;
}

template <>
payload_t BaseProtocol::serialize(const std::string& str) const {
    payload_t length = serialize(static_cast<uint16_t>(str.size()));

    payload_t payload;
    payload.reserve(length.size() + str.size());

    payload.insert(payload.end(), length.begin(), length.end());
    payload.insert(payload.end(), str.data(), str.data() + str.size());

    return payload;
}

template <typename T>
payload_t BaseProtocol::serialize(const std::optional<T>& v) const {
    payload_t payload;
    bool has_value = v.has_value();
    payload_t has_value_payload = serialize(has_value);
    payload.insert(payload.end(), has_value_payload.begin(), has_value_payload.end());

    if (has_value) {
        payload_t value_payload = serialize(v.value());
        payload.insert(payload.end(), value_payload.begin(), value_payload.end());
    }

    return payload;
}

template <>
payload_t BaseProtocol::serialize(const Vector2D& vec) const {
    payload_t payload;
    payload_t x_payload = serialize(vec.get_x());
    payload_t y_payload = serialize(vec.get_y());
    payload.insert(payload.end(), x_payload.begin(), x_payload.end());
    payload.insert(payload.end(), y_payload.begin(), y_payload.end());
    return payload;
}

template <>
payload_t BaseProtocol::serialize(const TimePoint& time_point) const {
    payload_t payload;
    payload.reserve(sizeof(uint64_t));
    uint64_t time_ns = htonl(time_point.time_since_epoch().count());
    payload.insert(payload.end(), reinterpret_cast<const char*>(&time_ns),
                   reinterpret_cast<const char*>(&time_ns) + sizeof(time_ns));
    return payload;
}

template <>
payload_t BaseProtocol::serialize(const GameInfo& game_info) const {
    payload_t payload;

    payload_t name_payload = serialize(game_info.name);
    payload_t map_name_payload = serialize(game_info.map_name);
    payload_t players_count_payload = serialize(game_info.players_count);
    payload_t phase_payload = serialize(game_info.phase);
    payload.reserve(name_payload.size() + map_name_payload.size() + players_count_payload.size() +
                    phase_payload.size());
    payload.insert(payload.end(), name_payload.begin(), name_payload.end());
    payload.insert(payload.end(), map_name_payload.begin(), map_name_payload.end());
    payload.insert(payload.end(), players_count_payload.begin(), players_count_payload.end());
    payload.insert(payload.end(), phase_payload.begin(), phase_payload.end());

    return payload;
}

template <>
payload_t BaseProtocol::serialize(const CharacterType& character_type) const {
    return serialize(static_cast<uint8_t>(character_type));
}

template <>
payload_t BaseProtocol::serialize(const ScoreboardEntry& entry) const {
    payload_t payload;

    payload_t money_payload = serialize(entry.money);
    payload_t kills_payload = serialize(entry.kills);
    payload_t deaths_payload = serialize(entry.deaths);
    payload_t score_payload = serialize(entry.score);

    payload.reserve(money_payload.size() + kills_payload.size() + deaths_payload.size() +
                    score_payload.size());
    payload.insert(payload.end(), money_payload.begin(), money_payload.end());
    payload.insert(payload.end(), kills_payload.begin(), kills_payload.end());
    payload.insert(payload.end(), deaths_payload.begin(), deaths_payload.end());
    payload.insert(payload.end(), score_payload.begin(), score_payload.end());

    return payload;
}

template <>
payload_t BaseProtocol::serialize(const Rectangle& rectangle) const {
    payload_t payload;

    payload_t pos_payload = serialize(rectangle.get_pos());
    payload_t width_payload = serialize(rectangle.get_width());
    payload_t height_payload = serialize(rectangle.get_height());
    payload_t rotation_payload = serialize(rectangle.get_rotation_deg());

    payload.reserve(pos_payload.size() + width_payload.size() + height_payload.size() +
                    rotation_payload.size());
    payload.insert(payload.end(), pos_payload.begin(), pos_payload.end());
    payload.insert(payload.end(), width_payload.begin(), width_payload.end());
    payload.insert(payload.end(), height_payload.begin(), height_payload.end());
    payload.insert(payload.end(), rotation_payload.begin(), rotation_payload.end());

    return payload;
}

template <>
payload_t BaseProtocol::serialize(const Tile& tile) const {
    payload_t payload;

    payload_t pos_payload = serialize(tile.pos);
    payload_t id_payload = serialize(tile.id);
    payload_t is_collidable_payload = serialize(tile.is_collidable);
    payload_t is_spawn_tt_payload = serialize(tile.is_spawn_tt);
    payload_t is_spawn_ct_payload = serialize(tile.is_spawn_ct);
    payload_t is_bomb_site_payload = serialize(tile.is_bomb_site);

    payload.reserve(pos_payload.size() + id_payload.size() + is_collidable_payload.size() +
                    is_spawn_tt_payload.size() + is_spawn_ct_payload.size() +
                    is_bomb_site_payload.size());

    payload.insert(payload.end(), pos_payload.begin(), pos_payload.end());
    payload.insert(payload.end(), id_payload.begin(), id_payload.end());
    payload.insert(payload.end(), is_collidable_payload.begin(), is_collidable_payload.end());
    payload.insert(payload.end(), is_spawn_tt_payload.begin(), is_spawn_tt_payload.end());
    payload.insert(payload.end(), is_spawn_ct_payload.begin(), is_spawn_ct_payload.end());
    payload.insert(payload.end(), is_bomb_site_payload.begin(), is_bomb_site_payload.end());

    return payload;
}

template <>
payload_t BaseProtocol::serialize(const Map& map) const {
    payload_t payload;

    payload_t name_payload = serialize(map.get_name());
    payload_t max_players_payload = serialize(map.get_max_players());
    payload_t height_payload = serialize(map.get_height());
    payload_t width_payload = serialize(map.get_width());
    payload_t tiles_payload = serialize(map.get_tiles());

    payload.reserve(name_payload.size() + max_players_payload.size() + tiles_payload.size());

    payload.insert(payload.end(), name_payload.begin(), name_payload.end());
    payload.insert(payload.end(), max_players_payload.begin(), max_players_payload.end());
    payload.insert(payload.end(), height_payload.begin(), height_payload.end());
    payload.insert(payload.end(), width_payload.begin(), width_payload.end());
    payload.insert(payload.end(), tiles_payload.begin(), tiles_payload.end());

    return payload;
}

template <>
payload_t BaseProtocol::serialize(const WorldItem<GunType>& world_item) const {
    payload_t payload;
    payload_t gun_payload = serialize(world_item.item);
    payload_t hitbox_payload = serialize(world_item.hitbox);

    payload.reserve(gun_payload.size() + hitbox_payload.size());
    payload.insert(payload.end(), gun_payload.begin(), gun_payload.end());
    payload.insert(payload.end(), hitbox_payload.begin(), hitbox_payload.end());

    return payload;
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
        payload_t attr##_payload = serialize(update.get_##attr());                   \
        payload.insert(payload.end(), attr##_payload.begin(), attr##_payload.end()); \
    }
#define V_SERIALIZE_UPDATE(type, attr)                                               \
    payload.push_back(update.has_##attr##_changed());                                \
    if (update.has_##attr##_changed()) {                                             \
        payload_t attr##_payload = serialize(update.get_##attr());                   \
        payload.insert(payload.end(), attr##_payload.begin(), attr##_payload.end()); \
    }

#define SERIALIZE_UPDATE(CLASS, ATTRS)                                    \
    template <>                                                           \
    payload_t BaseProtocol::serialize<CLASS>(const CLASS& update) const { \
        payload_t payload;                                                \
                                                                          \
        ATTRS(X_SERIALIZE_UPDATE, M_SERIALIZE_UPDATE, V_SERIALIZE_UPDATE) \
                                                                          \
        return payload;                                                   \
    }

SERIALIZE_UPDATE(BombUpdate, BOMB_ATTRS)
SERIALIZE_UPDATE(KnifeUpdate, KNIFE_ATTRS)
SERIALIZE_UPDATE(GunUpdate, GUN_ATTRS)
SERIALIZE_UPDATE(InventoryUpdate, INVENTORY_ATTRS)
SERIALIZE_UPDATE(PlayerUpdate, PLAYER_ATTRS)
SERIALIZE_UPDATE(PhaseUpdate, PHASE_ATTRS)
// SERIALIZE_UPDATE(GameUpdate, GAME_ATTRS)

template <>
payload_t BaseProtocol::serialize<GameUpdate>(const GameUpdate& update) const {
    payload_t payload;
    payload.push_back(update.has_phase_changed());
    if (update.has_phase_changed()) {
        payload_t phase_payload = serialize(update.get_phase());
        payload.insert(payload.end(), phase_payload.begin(), phase_payload.end());
    }
    payload.push_back(update.has_num_rounds_changed());
    if (update.has_num_rounds_changed()) {
        payload_t num_rounds_payload = serialize(update.get_num_rounds());
        payload.insert(payload.end(), num_rounds_payload.begin(), num_rounds_payload.end());
    }
    payload.push_back(update.has_players_changed());
    if (update.has_players_changed()) {
        payload_t players_payload = serialize(update.get_players());
        payload.insert(payload.end(), players_payload.begin(), players_payload.end());
    }
    payload.push_back(update.has_dropped_guns_changed());
    if (update.has_dropped_guns_changed()) {
        payload_t dropped_guns_payload = serialize(update.get_dropped_guns());
        payload.insert(payload.end(), dropped_guns_payload.begin(), dropped_guns_payload.end());
    }
    payload.push_back(update.has_bomb_changed());
    if (update.has_bomb_changed()) {
        payload_t bomb_payload = serialize(update.get_bomb());
        payload.insert(payload.end(), bomb_payload.begin(), bomb_payload.end());
    }
    return payload;
}

template <>
payload_t BaseProtocol::serialize(const WorldItem<BombUpdate>& world_item) const {
    payload_t payload;

    payload_t bomb_payload = serialize(world_item.item);
    payload_t hitbox_payload = serialize(world_item.hitbox);
    payload.reserve(bomb_payload.size() + hitbox_payload.size());

    payload.insert(payload.end(), bomb_payload.begin(), bomb_payload.end());
    payload.insert(payload.end(), hitbox_payload.begin(), hitbox_payload.end());

    return payload;
}

// ---------- Deserialization ----------

payload_t BaseProtocol::pop(payload_t& payload, size_t size) const {
    if (size > payload.size()) {
        throw std::runtime_error("BaseProtocol::pop: Requested size (" + std::to_string(size) +
                                 ") exceeds payload size (" + std::to_string(payload.size()) + ")");
    }
    payload_t result(payload.begin(), payload.begin() + size);
    payload.erase(payload.begin(), payload.begin() + size);
    return result;
}

template <>
uint8_t BaseProtocol::deserialize<uint8_t>(payload_t& payload) const {
    payload_t data = pop(payload, sizeof(uint8_t));
    return static_cast<uint8_t>(data[0]);
}

template <>
uint16_t BaseProtocol::deserialize<uint16_t>(payload_t& payload) const {
    payload_t data = pop(payload, sizeof(uint16_t));
    return ntohs(*reinterpret_cast<const uint16_t*>(data.data()));
}

template <>
int16_t BaseProtocol::deserialize<int16_t>(payload_t& payload) const {
    payload_t data = pop(payload, sizeof(int16_t));
    return ntohs(*reinterpret_cast<const int16_t*>(data.data()));
}

template <>
std::string BaseProtocol::deserialize<std::string>(payload_t& payload) const {
    uint16_t length = deserialize<uint16_t>(payload);

    payload_t data = pop(payload, length);

    return std::string(data.data(), data.size());
}

template <>
int BaseProtocol::deserialize<int>(payload_t& payload) const {
    return static_cast<int>(deserialize<int16_t>(payload));
}

template <>
bool BaseProtocol::deserialize<bool>(payload_t& payload) const {
    payload_t data = pop(payload, sizeof(uint8_t));
    return static_cast<bool>(data[0]);
}

template <>
Vector2D BaseProtocol::deserialize<Vector2D>(payload_t& payload) const {
    int x = deserialize<int>(payload);
    int y = deserialize<int>(payload);
    return Vector2D(x, y);
}

template <>
TimePoint BaseProtocol::deserialize<TimePoint>(payload_t& payload) const {
    payload_t data = pop(payload, sizeof(uint64_t));
    uint64_t time_ns = ntohl(*reinterpret_cast<const uint64_t*>(data.data()));
    return TimePoint(std::chrono::nanoseconds(time_ns));
}

template <>
GameInfo BaseProtocol::deserialize<GameInfo>(payload_t& payload) const {
    std::string name = deserialize<std::string>(payload);
    std::string map_name = deserialize<std::string>(payload);
    int players_count = deserialize<int>(payload);
    PhaseType phase = deserialize<PhaseType>(payload);
    return GameInfo(name, map_name, players_count, phase);
}


template <>
ScoreboardEntry BaseProtocol::deserialize<ScoreboardEntry>(payload_t& payload) const {
    int money = deserialize<int>(payload);
    int kills = deserialize<int>(payload);
    int deaths = deserialize<int>(payload);
    int score = deserialize<int>(payload);

    return ScoreboardEntry(money, kills, deaths, score);
}

template <>
Rectangle BaseProtocol::deserialize<Rectangle>(payload_t& payload) const {
    Vector2D pos = deserialize<Vector2D>(payload);
    int width = deserialize<int>(payload);
    int height = deserialize<int>(payload);
    int rotation_deg = deserialize<int>(payload);

    Rectangle rectangle(pos, width, height);
    rectangle.rotate(rotation_deg);
    return rectangle;
}

template <>
Tile BaseProtocol::deserialize<Tile>(payload_t& payload) const {
    Vector2D pos = deserialize<Vector2D>(payload);
    int id = deserialize<int>(payload);
    bool is_collidable = deserialize<bool>(payload);
    bool is_spawn_tt = deserialize<bool>(payload);
    bool is_spawn_ct = deserialize<bool>(payload);
    bool is_bomb_site = deserialize<bool>(payload);

    return Tile{std::move(pos), id, is_collidable, is_spawn_tt, is_spawn_ct, is_bomb_site};
}

template <>
Map BaseProtocol::deserialize<Map>(payload_t& payload) const {
    std::string name = deserialize<std::string>(payload);
    int max_players = deserialize<int>(payload);
    int height = deserialize<int>(payload);
    int width = deserialize<int>(payload);

    Map map(name, max_players, height, width);

    auto tiles = deserialize<std::vector<std::vector<std::optional<Tile>>>>(payload);
    for (auto& row: tiles) {
        for (auto& tile: row) {
            if (tile) {
                map.add_tile(std::move(tile.value()));
            }
        }
    }

    return map;
}

template <>
WorldItem<GunType> BaseProtocol::deserialize<WorldItem<GunType>>(payload_t& payload) const {
    GunType gun_type = deserialize<GunType>(payload);
    Rectangle hitbox = deserialize<Rectangle>(payload);

    return WorldItem<GunType>{gun_type, hitbox};
}

#define X_DESERIALIZE_UPDATE(type, attr)        \
    if (deserialize<bool>(payload)) {           \
        type attr = deserialize<type>(payload); \
        result.set_##attr(attr);                \
    }
#define M_DESERIALIZE_UPDATE(key_type, value_type, attr)                                  \
    if (deserialize<bool>(payload)) {                                                     \
        std::map<key_type, value_type> attr = deserialize<key_type, value_type>(payload); \
        result.set_##attr(attr);                                                          \
    }
#define V_DESERIALIZE_UPDATE(type, attr)                                  \
    if (deserialize<bool>(payload)) {                                     \
        std::vector<type> attr = deserialize<std::vector<type>>(payload); \
        result.set_##attr(attr);                                          \
    }

#define DESERIALIZE_UPDATE(CLASS, ATTRS)                                        \
    template <>                                                                 \
    CLASS BaseProtocol::deserialize<CLASS>(payload_t & payload) const {         \
        CLASS result;                                                           \
                                                                                \
        ATTRS(X_DESERIALIZE_UPDATE, M_DESERIALIZE_UPDATE, V_DESERIALIZE_UPDATE) \
                                                                                \
        return result;                                                          \
    }

DESERIALIZE_UPDATE(BombUpdate, BOMB_ATTRS)
DESERIALIZE_UPDATE(KnifeUpdate, KNIFE_ATTRS)
DESERIALIZE_UPDATE(GunUpdate, GUN_ATTRS)
DESERIALIZE_UPDATE(InventoryUpdate, INVENTORY_ATTRS)
DESERIALIZE_UPDATE(PlayerUpdate, PLAYER_ATTRS)
DESERIALIZE_UPDATE(PhaseUpdate, PHASE_ATTRS)
// DESERIALIZE_UPDATE(GameUpdate, GAME_ATTRS)

template <>
GameUpdate BaseProtocol::deserialize<GameUpdate>(payload_t& payload) const {
    GameUpdate result;
    if (deserialize<bool>(payload)) {
        PhaseUpdate phase = deserialize<PhaseUpdate>(payload);
        result.set_phase(phase);
    }
    if (deserialize<bool>(payload)) {
        int num_rounds = deserialize<int>(payload);
        result.set_num_rounds(num_rounds);
    }
    if (deserialize<bool>(payload)) {
        std::map<std::string, PlayerUpdate> players =
                deserialize<std::string, PlayerUpdate>(payload);
        result.set_players(players);
    }
    if (deserialize<bool>(payload)) {
        std::vector<WorldItem<GunType>> dropped_guns =
                deserialize<std::vector<WorldItem<GunType>>>(payload);
        result.set_dropped_guns(dropped_guns);
    }
    if (deserialize<bool>(payload)) {
        std::optional<WorldItem<BombUpdate>> bomb =
                deserialize<std::optional<WorldItem<BombUpdate>>>(payload);
        result.set_bomb(bomb);
    }
    return result;
}


template <>
WorldItem<BombUpdate> BaseProtocol::deserialize<WorldItem<BombUpdate>>(payload_t& payload) const {
    BombUpdate bomb_update = deserialize<BombUpdate>(payload);
    Rectangle hitbox = deserialize<Rectangle>(payload);

    return WorldItem<BombUpdate>{bomb_update, hitbox};
}

Message BaseProtocol::recv() {
    payload_t header(sizeof(uint8_t) + sizeof(uint16_t));
    socket->recvall(header.data(), sizeof(uint8_t) + sizeof(uint16_t));

    MessageType msg_type = deserialize<MessageType>(header);
    uint16_t length = deserialize<uint16_t>(header);

    payload_t content(length);
    socket->recvall(content.data(), length);

    Message message = deserialize_message(msg_type, content);

    if (content.size() > 0) {
        throw std::runtime_error("BaseProtocol::recv: Not all data was consumed, remaining size: " +
                                 std::to_string(content.size()));
    }

    return message;
}

void BaseProtocol::send(const Message& message) {
    payload_t payload;
    payload_t header = serialize(message.get_type());
    payload_t content = serialize_message(message);  // TODO replace with a function pointer
    payload_t length = serialize(static_cast<uint16_t>(content.size()));

    payload.reserve(header.size() + length.size() + content.size());
    payload.insert(payload.end(), header.begin(), header.end());
    payload.insert(payload.end(), length.begin(), length.end());
    payload.insert(payload.end(), content.begin(), content.end());

    if (payload.size() == 0)
        throw std::runtime_error("BaseProtocol: Empty payload");

    socket->sendall(payload.data(), payload.size());
}

void BaseProtocol::close() {
    if (is_closed())
        return;

    socket->shutdown(SHUT_RDWR);
    socket->close();
}

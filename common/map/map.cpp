#include "common/map/map.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

Map::Map(const std::string& name, int max_players): name(name), max_players(max_players) {}

void Map::validate() const {
    if (tiles.empty()) {
        throw std::runtime_error("Map '" + name + "' has no tiles");
    }
    if (spawns_tts.empty()) {
        throw std::runtime_error("Map '" + name + "' has no Terrorist spawns");
    }
    if (spawns_cts.empty()) {
        throw std::runtime_error("Map '" + name + "' has no Counter-Terrorist spawns");
    }
}

int Map::get_max_players() const { return max_players; }

const std::vector<Tile>& Map::get_tiles() const { return tiles; }

void Map::add_tile(MapTileType type, Vector2D&& grid_pos) {
    tiles.emplace_back(type, std::move(grid_pos));
}

void Map::add_spawn_tt(Vector2D&& grid_pos) { spawns_tts.push_back(std::move(grid_pos)); }

void Map::add_spawn_ct(Vector2D&& grid_pos) { spawns_cts.push_back(std::move(grid_pos)); }

void Map::add_bomb_site(Vector2D&& grid_pos) { bomb_sites.push_back(std::move(grid_pos)); }

Vector2D Map::random_spawn_tt_pos() const { return random_spawn_pos(spawns_tts); }

Vector2D Map::random_spawn_ct_pos() const { return random_spawn_pos(spawns_cts); }

Vector2D Map::random_spawn_pos(const std::vector<Vector2D>& spawns) const {
    int rand_idx = std::rand() % spawns.size();
    return spawns.at(rand_idx);
}

bool Map::is_spawn_tt_pos(const Vector2D& pos) const { return is_pos_in_vector(pos, spawns_tts); }

bool Map::is_spawn_ct_pos(const Vector2D& pos) const { return is_pos_in_vector(pos, spawns_cts); }

bool Map::is_pos_in_vector(const Vector2D& pos, const std::vector<Vector2D>& vector) const {
    return std::any_of(vector.begin(), vector.end(), [&](const Vector2D& p) { return p == pos; });
}

Map::~Map() {}

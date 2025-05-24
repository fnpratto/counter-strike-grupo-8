#include "map.h"

#include <algorithm>
#include <utility>

#include "server/errors.h"

Map::Map(const std::string& name): name(name) {}

bool Map::is_collidable(const Vector2D& pos) {
    auto it = std::find_if(tiles.begin(), tiles.end(),
                           [&pos](const auto& tile) { return tile.get_pos() == pos; });
    if (it != tiles.end())
        return it->is_collidable();

    throw InvalidMapPosition();
}

bool Map::is_spawn_tt(const Vector2D& pos) { return is_pos_in_vector(spawns_tts, pos); }

bool Map::is_spawn_ct(const Vector2D& pos) { return is_pos_in_vector(spawns_cts, pos); }

bool Map::is_bomb_site(const Vector2D& pos) { return is_pos_in_vector(bomb_sites, pos); }

bool Map::is_pos_in_vector(const std::vector<Vector2D>& vector, const Vector2D& pos) {
    return std::any_of(vector.begin(), vector.end(), [&pos](const auto& p) { return p == pos; });
}

void Map::add_tile(Tile&& tile) { tiles.push_back(std::move(tile)); }

void Map::add_spawn_tt(Vector2D&& pos) { spawns_tts.push_back(std::move(pos)); }

void Map::add_spawn_ct(Vector2D&& pos) { spawns_cts.push_back(std::move(pos)); }

void Map::add_bomb_site(Vector2D&& pos) { bomb_sites.push_back(std::move(pos)); }

Vector2D Map::random_spawn_tt_pos() const { return random_spawn_pos(spawns_tts); }

Vector2D Map::random_spawn_ct_pos() const { return random_spawn_pos(spawns_cts); }

Vector2D Map::random_spawn_pos(const std::vector<Vector2D>& spawns) const {
    int rand_idx = std::rand() % spawns.size();
    return spawns.at(rand_idx);
}

Map::~Map() {}

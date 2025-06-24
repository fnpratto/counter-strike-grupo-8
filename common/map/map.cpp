#include "map.h"

#include <functional>
#include <stdexcept>
#include <utility>

#include "common/physics/physics_config.h"

Map::Map(const std::string& name, int max_players, int height, int width):
        name(name),
        max_players(max_players),
        height(height),
        width(width),
        tiles(width, std::vector<std::optional<Tile>>(height)) {}

std::string Map::get_name() const { return name; }

int Map::get_max_players() const { return max_players; }

int Map::get_height() const { return height; }

int Map::get_width() const { return width; }

const std::vector<std::vector<std::optional<Tile>>>& Map::get_tiles() const { return tiles; }

const std::vector<std::reference_wrapper<Tile>>& Map::get_collidables() const {
    return collidables;
}

const std::vector<std::reference_wrapper<Tile>>& Map::get_spawns_tts() const { return spawns_tts; }

const std::vector<std::reference_wrapper<Tile>>& Map::get_spawns_cts() const { return spawns_cts; }

const std::vector<std::reference_wrapper<Tile>>& Map::get_bomb_sites() const { return bomb_sites; }

const std::vector<std::pair<GunType, Vector2D>>& Map::get_guns() const { return guns; }

void Map::validate() const {
    if (spawns_tts.empty())
        throw std::runtime_error("Map '" + name + "' has no Terrorist spawns");
    if (spawns_cts.empty())
        throw std::runtime_error("Map '" + name + "' has no Counter-Terrorist spawns");
}

void Map::add_tile(Tile&& tile) {
    int x = tile.pos.get_x() / PhysicsConfig::meter_size;
    int y = tile.pos.get_y() / PhysicsConfig::meter_size;
    tiles[x][y] = std::move(tile);
    Tile& added_tile = tiles.at(x).at(y).value();
    if (added_tile.is_collidable)
        collidables.emplace_back(added_tile);
    if (added_tile.is_spawn_tt)
        spawns_tts.emplace_back(added_tile);
    if (added_tile.is_spawn_ct)
        spawns_cts.emplace_back(added_tile);
    if (added_tile.is_bomb_site)
        bomb_sites.emplace_back(added_tile);
}

void Map::add_gun(GunType gun_type, const Vector2D& pos) {
    guns.push_back(std::make_pair(gun_type, pos));
}

#include "map.h"

#include <stdexcept>
#include <utility>

Map::Map(const std::string& name, int max_players): name(name), max_players(max_players) {}

const std::map<Vector2D, const Tile>& Map::get_tiles() const { return tiles; }

const std::vector<const Tile&>& Map::get_spawns_tts() const { return spawns_tts; }

const std::vector<const Tile&>& Map::get_spawns_cts() const { return spawns_cts; }

void Map::validate() const {
    if (spawns_tts.empty())
        throw std::runtime_error("Map '" + name + "' has no Terrorist spawns");
    if (spawns_cts.empty())
        throw std::runtime_error("Map '" + name + "' has no Counter-Terrorist spawns");
}

void Map::add_tile(Tile&& tile) {
    tiles.emplace(tile.pos, std::move(tile));
    if (tile.is_spawn_tt)
        spawns_tts.push_back(tiles.at(tile.pos));
    if (tile.is_spawn_ct)
        spawns_cts.push_back(tiles.at(tile.pos));
}

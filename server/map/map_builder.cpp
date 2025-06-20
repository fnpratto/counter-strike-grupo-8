#include "map_builder.h"

#include "common/map/tile.h"
#include "common/utils/vector_2d.h"

MapBuilder::MapBuilder(const std::string& filename): filename(filename) {}

Map MapBuilder::build() {
    YAML::Node map_data = YAML::LoadFile(filename);

    std::string name = map_data["name"].as<std::string>();
    int max_players = map_data["max_players"].as<int>();
    Map map(name, max_players);

    YAML::Node tiles = map_data["tiles"];
    load_tiles(tiles, map);

    map.validate();
    return map;
}

void MapBuilder::load_tiles(const YAML::Node& tiles, Map& map) {
    if (!tiles || !tiles.IsMap())
        return;

    for (const auto& tile_data: tiles) {
        int x = tile_data["x"].as<int>();
        int y = tile_data["y"].as<int>();
        int id = tile_data["id"].as<int>();
        bool is_collidable = tile_data["is_collidable"].as<bool>();
        bool is_spawn_tt = tile_data["is_spawn_tt"].as<bool>();
        bool is_spawn_ct = tile_data["is_spawn_ct"].as<bool>();
        bool is_bomb_site = tile_data["is_bomb_site"].as<bool>();

        Vector2D pos(x, y);
        map.add_tile(Tile{pos, id, is_collidable, is_spawn_tt, is_spawn_ct, is_bomb_site});
    }
}

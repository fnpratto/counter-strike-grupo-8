#include "map_builder.h"

#include "common/map/tile.h"
#include "common/models.h"
#include "common/physics/physics_config.h"
#include "common/utils/vector_2d.h"

MapBuilder::MapBuilder(const std::string& filename): filename(filename) {}

Map MapBuilder::build() {
    YAML::Node map_data = YAML::LoadFile(filename);

    std::string name = map_data["name"].as<std::string>();
    int max_players = map_data["max_players"].as<int>();
    int height = map_data["height"].as<int>();
    int width = map_data["width"].as<int>();
    Map map(name, max_players, height, width);

    YAML::Node tiles = map_data["tiles"];
    load_tiles(tiles, map);

    YAML::Node guns = map_data["guns"];
    load_guns(guns, map);

    map.validate();
    return map;
}

void MapBuilder::load_tiles(const YAML::Node& tiles, Map& map) {
    if (!tiles || !tiles.IsSequence())
        return;

    for (const auto& tile_data: tiles) {
        int x = tile_data["x"].as<int>();
        int y = tile_data["y"].as<int>();
        int id = tile_data["id"].as<int>();
        bool is_collidable = tile_data["is_collidable"].as<bool>();
        bool is_spawn_tt = tile_data["is_spawn_tt"].as<bool>();
        bool is_spawn_ct = tile_data["is_spawn_ct"].as<bool>();
        bool is_bomb_site = tile_data["is_bomb_site"].as<bool>();

        Vector2D pos = Vector2D(x, y) * PhysicsConfig::meter_size;
        map.add_tile(Tile{pos, id, is_collidable, is_spawn_tt, is_spawn_ct, is_bomb_site});
    }
}

void MapBuilder::load_guns(const YAML::Node& guns, Map& map) {
    if (!guns || !guns.IsSequence())
        return;

    for (const auto& gun_data: guns) {
        int x = gun_data["x"].as<int>();
        int y = gun_data["y"].as<int>();
        GunType gun_type = static_cast<GunType>(gun_data["type"].as<int>());

        Vector2D pos = Vector2D(x, y) * PhysicsConfig::meter_size;
        map.add_gun(gun_type, pos);
    }
}

#pragma once

#include <string>
#include <utility>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "common/models.h"
#include "server/map/tile.h"
#include "server/utils/vector_2d.h"

#include "map.h"

// TODO: Make subclasses of MapBuilder that build specific
//       maps such as DesertMapBuilder, AztecMapBuilder, and
//       TrainingMapBuilder from YAML files.

class MapBuilder {
private:
    const std::string& filename;

public:
    explicit MapBuilder(const std::string& filename): filename(filename) {}

    Map build() {
        YAML::Node map_data = YAML::LoadFile(filename);

        std::string name = map_data["name"].as<std::string>();
        Map map(name);

        YAML::Node tiles = map_data["tiles"];
        if (tiles["floors"]) {
            for (const auto& tile_data: tiles["floors"]) {
                int x = tile_data["x"].as<int>();
                int y = tile_data["y"].as<int>();
                Vector2D tile_pos(x, y);
                Tile floor(MapTileType::Floor);
                map.add_tile(std::move(floor));
            }
        }

        if (tiles["walls"]) {
            for (const auto& tile_data: tiles["walls"]) {
                int x = tile_data["x"].as<int>();
                int y = tile_data["y"].as<int>();
                Vector2D tile_pos(x, y);
                Tile wall(MapTileType::Wall);
                map.add_tile(std::move(wall));
            }
        }

        if (tiles["boxes"]) {
            for (const auto& tile_data: tiles["walls"]) {
                int x = tile_data["x"].as<int>();
                int y = tile_data["y"].as<int>();
                Vector2D tile_pos(x, y);
                Tile box(MapTileType::Box);
                map.add_tile(std::move(box));
            }
        }

        for (const auto& spawn_tt_data: map_data["spawns_tts"]) {
            float x = spawn_tt_data["x"].as<float>();
            float y = spawn_tt_data["y"].as<float>();
            Vector2D spawn_tt_pos(x, y);
            map.add_spawn_tt(std::move(spawn_tt_pos));
        }

        for (const auto& spawn_ct_data: map_data["spawns_cts"]) {
            float x = spawn_ct_data["x"].as<float>();
            float y = spawn_ct_data["y"].as<float>();
            Vector2D spawn_ct_pos(x, y);
            map.add_spawn_ct(std::move(spawn_ct_pos));
        }

        for (const auto& bomb_site_data: map_data["bomb_sites"]) {
            float x = bomb_site_data["x"].as<float>();
            float y = bomb_site_data["y"].as<float>();
            Vector2D bomb_site_pos(x, y);
            map.add_bomb_site(std::move(bomb_site_pos));
        }

        return map;
    }
};

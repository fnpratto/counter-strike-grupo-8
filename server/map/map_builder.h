#pragma once

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

#include "map.h"
#include "common/models.h"
#include "server/utils/vector_2d.h"

class MapBuilder {
private:
    const std::string& filename;

public:
    explicit MapBuilder(const std::string& filename) :
            filename(filename) {}

    Map build() { 
        YAML::Node map_data = YAML::LoadFile(filename);
        
        std::string name = map_data["name"].as<std::string>();
        Map map(std::move(name));
        
        for (const auto& tile_row_data : map_data["tiles"]) {
            std::vector<MapTileType> tiles_row;
            for (const auto& tile_val : tile_row_data) {
                MapTileType tile = tile_val.as<MapTileType>();
                tiles_row.push_back(tile);
            }
            map.add_tiles_row(std::move(tiles_row));
        }

        for (const auto& spawn_tt_data : map_data["spawns_tts"]) {
            float spawn_tt_x = spawn_tt_data["x"].as<float>();
            float spawn_tt_y = spawn_tt_data["y"].as<float>();
            Vector2D spawn_tt(spawn_tt_x, spawn_tt_y);
            map.add_spawn_tt(std::move(spawn_tt));
        }

        for (const auto& spawn_ct_data : map_data["spawns_cts"]) {
            float spawn_ct_x = spawn_ct_data["x"].as<float>();
            float spawn_ct_y = spawn_ct_data["y"].as<float>();
            Vector2D spawn_ct(spawn_ct_x, spawn_ct_y);
            map.add_spawn_ct(std::move(spawn_ct));
        }

        for (const auto& bomb_site_data : map_data["bomb_sites"]) {
            float bomb_site_x = bomb_site_data["x"].as<float>();
            float bomb_site_y = bomb_site_data["y"].as<float>();
            Vector2D bomb_site(bomb_site_x, bomb_site_y);
            map.add_bomb_site(std::move(bomb_site));
        }

        return map; 
    }
};

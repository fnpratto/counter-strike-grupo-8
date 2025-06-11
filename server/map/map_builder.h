#pragma once

#include <string>
#include <utility>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "common/models.h"
#include "common/utils/vector_2d.h"
#include "server/physics/physics_config.h"

#include "map.h"

// TODO: Make subclasses of MapBuilder that build specific
//       maps such as DesertMapBuilder, AztecMapBuilder, and
//       TrainingMapBuilder from YAML files.

class MapBuilder {
private:
    std::string filename;

public:
    explicit MapBuilder(const std::string& filename): filename(filename) {}

    Map build() {
        YAML::Node map_data = YAML::LoadFile(filename);

        std::string name = map_data["name"].as<std::string>();
        int max_players = map_data["max_players"].as<int>();
        Map map(name, max_players);

        YAML::Node tiles = map_data["tiles"];
        if (tiles["floors"]) {
            for (const auto& tile_data: tiles["floors"]) {
                int x = tile_data["x"].as<int>();
                int y = tile_data["y"].as<int>();
                Vector2D tile_pos(x, y);
                map.add_floor(std::move(tile_pos * PhysicsConfig::meter_size));
            }
        }

        if (tiles["walls"]) {
            for (const auto& tile_data: tiles["walls"]) {
                int x = tile_data["x"].as<int>();
                int y = tile_data["y"].as<int>();
                Vector2D tile_pos(x, y);
                map.add_wall(std::move(tile_pos * PhysicsConfig::meter_size));
            }
        }

        if (tiles["boxes"]) {
            for (const auto& tile_data: tiles["boxes"]) {
                int x = tile_data["x"].as<int>();
                int y = tile_data["y"].as<int>();
                Vector2D tile_pos(x, y);
                map.add_box(std::move(tile_pos * PhysicsConfig::meter_size));
            }
        }

        for (const auto& spawn_tt_data: map_data["spawns_tts"]) {
            int x = spawn_tt_data["x"].as<int>();
            int y = spawn_tt_data["y"].as<int>();
            Vector2D spawn_tt_pos(x, y);
            map.add_spawn_tt(std::move(spawn_tt_pos * PhysicsConfig::meter_size));
        }

        for (const auto& spawn_ct_data: map_data["spawns_cts"]) {
            int x = spawn_ct_data["x"].as<int>();
            int y = spawn_ct_data["y"].as<int>();
            Vector2D spawn_ct_pos(x, y);
            map.add_spawn_ct(std::move(spawn_ct_pos * PhysicsConfig::meter_size));
        }

        for (const auto& bomb_site_data: map_data["bomb_sites"]) {
            int x = bomb_site_data["x"].as<int>();
            int y = bomb_site_data["y"].as<int>();
            Vector2D bomb_site_pos(x, y);
            map.add_bomb_site(std::move(bomb_site_pos * PhysicsConfig::meter_size));
        }

        map.validate();

        return map;
    }
};

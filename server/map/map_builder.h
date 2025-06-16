#pragma once

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "common/map/map.h"
#include "common/models.h"
#include "common/physics/physics_config.h"
#include "common/utils/vector_2d.h"

// TODO: Make subclasses of MapBuilder that build specific
//       maps such as DesertMapBuilder, AztecMapBuilder, and
//       TrainingMapBuilder from YAML files.

class MapBuilder {
private:
    std::string filename;

    template <typename T>
    void load_positions(YAML::Node node, std::vector<T>& vector) {
        if (!node || !node.IsSequence())
            return;
        for (const auto& data: node) {
            int x = data["x"].as<int>();
            int y = data["y"].as<int>();
            Vector2D pos(x, y);
            vector.push_back(T(std::move(pos * PhysicsConfig::meter_size)));
        }
    }

public:
    explicit MapBuilder(const std::string& filename): filename(filename) {}

    Map build() {
        YAML::Node map_data = YAML::LoadFile(filename);

        std::string name = map_data["name"].as<std::string>();
        int max_players = map_data["max_players"].as<int>();
        Map map(name, max_players);

        YAML::Node tiles = map_data["tiles"];
        if (tiles["floors"])
            load_positions<Floor>(tiles["floors"], map.floors);
        if (tiles["walls"])
            load_positions<Wall>(tiles["walls"], map.walls);
        if (tiles["boxes"])
            load_positions<Box>(tiles["boxes"], map.boxes);

        load_positions<Vector2D>(map_data["spawns_tts"], map.spawns_tts);
        load_positions<Vector2D>(map_data["spawns_cts"], map.spawns_cts);
        load_positions<Vector2D>(map_data["bomb_sites"], map.bomb_sites);

        map.validate();
        return map;
    }
};

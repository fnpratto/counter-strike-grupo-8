#pragma once

#include <string>

#include <yaml-cpp/yaml.h>

#include "common/map/map.h"

// TODO: Make MapBuilder build specific maps such as
//       DesertMapBuilder, AztecMapBuilder, and
//       TrainingMapBuilder from YAML files.

class MapBuilder {
private:
    std::string filename;

    void load_tiles(const YAML::Node& tiles, Map& map);

public:
    explicit MapBuilder(const std::string& filename);

    Map build();
};

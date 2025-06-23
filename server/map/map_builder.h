#pragma once

#include <string>

#include <yaml-cpp/yaml.h>

#include "common/map/map.h"

class MapBuilder {
private:
    std::string filename;

    void load_tiles(const YAML::Node& tiles, Map& map);

public:
    explicit MapBuilder(const std::string& filename);

    Map build();
};

#pragma once

#include <filesystem>
#include <map>
#include <string>

#include <yaml-cpp/yaml.h>

#include "common/map/map.h"


struct TileSheet {
    std::string sheet_path;
    std::map<int, Vector2D> tiles;

    explicit TileSheet(const std::string& sheet_path): sheet_path(sheet_path) {}
};

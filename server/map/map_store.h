#pragma once

#include <map>
#include <string>
#include <vector>

#include "common/map/map.h"

#include "map_builder.h"

class MapStore {
    static constexpr const char* MAPS_DIR = "./maps/";

    std::map<std::string, Map> maps;

public:
    MapStore();

    Map get_map(const std::string& map_name) const;
    std::vector<std::string> get_map_names() const;

    MapStore(const MapStore&) = delete;
    MapStore& operator=(const MapStore&) = delete;
};

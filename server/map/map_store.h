#pragma once

#include <map>
#include <string>

#include "common/map/map.h"

#include "map_builder.h"

class MapStore {
    static constexpr const char* MAPS_DIR = "maps/";

    std::map<int, Map> maps;

public:
    MapStore();

    Map get_map(int map_id) const;
    std::map<std::string, int> get_maps_info() const;

    MapStore(const MapStore&) = delete;
    MapStore& operator=(const MapStore&) = delete;
};

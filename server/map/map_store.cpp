#include "map_store.h"

#include <filesystem>
#include <map>
#include <string>
#include <utility>

#include "common/map/map.h"

#include "map_builder.h"

MapStore::MapStore() {
    // Load all map files from MAPS_DIR
    for (const auto& map_file: std::filesystem::directory_iterator(MAPS_DIR)) {
        if (map_file.path().extension() == ".yaml") {
            MapBuilder builder(map_file.path().string());
            Map map = builder.build();
            maps.emplace(maps.size(), std::move(map));
        }
    }
}

Map MapStore::get_map(int map_id) const {
    auto it = maps.find(map_id);
    if (it == maps.end())
        throw std::runtime_error("Map with ID " + std::to_string(map_id) + " not found");
    return it->second;
}

std::map<std::string, int> MapStore::get_maps_info() const {
    std::map<std::string, int> info;
    for (const auto& [id, map]: maps) {  // cppcheck-suppress[unassignedVariable]
        info[map.name] = id;
    }
    return info;
}

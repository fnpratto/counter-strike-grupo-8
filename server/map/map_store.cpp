#include "map_store.h"

#include <filesystem>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "common/map/map.h"

#include "map_builder.h"

MapStore::MapStore() {
    // Load all map files from MAPS_DIR
    for (const auto& map_file: std::filesystem::directory_iterator(MAPS_DIR)) {
        if (map_file.path().extension() == ".yaml") {
            MapBuilder builder(map_file.path().string());
            Map map = builder.build();
            maps.emplace(map.name, std::move(map));
        }
    }
}

// TODO: This should "cache" the maps in memory, so that they are not loaded from disk every time.
Map MapStore::get_map(const std::string& map_name) const {
    auto it = maps.find(map_name);
    if (it == maps.end())
        throw std::runtime_error("Map with name " + map_name + " not found");
    return it->second;
}

std::vector<std::string> MapStore::get_map_names() const {
    std::vector<std::string> names;
    for (const auto& [name, _]: maps) {
        names.push_back(name);
    }
    return names;
}

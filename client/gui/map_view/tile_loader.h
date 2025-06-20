#pragma once

#include <filesystem>
#include <set>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "common/map/map.h"

#include "sdl_tile.h"

class TileLoader {
    static constexpr const char* TILE_SHEETS_DIR = "../assets/tile_sheets";

    std::vector<TileSheet> sheets;

public:
    explicit TileLoader(const Map& map) {
        // Collect all unique tile IDs present in the map
        std::set<int> required_tile_ids;
        const auto& map_tiles = map.get_tiles();
        for (const auto& row: map_tiles) {
            for (const auto& tile_opt: row) {
                if (tile_opt.has_value()) {
                    required_tile_ids.insert(tile_opt->id);
                }
            }
        }

        for (const auto& tile_file: std::filesystem::directory_iterator(TILE_SHEETS_DIR)) {
            if (tile_file.path().extension() != ".yaml")
                continue;

            YAML::Node node = YAML::LoadFile(tile_file.path().string());

            std::string sheet_path = node["path"].as<std::string>();

            TileSheet tile_sheet(sheet_path);
            int tile_size = node["tile_size"].as<int>();

            for (const auto& tile_node: node["tiles"]) {
                int id = tile_node["id"].as<int>();

                // Only load tiles that are actually used in the map
                if (required_tile_ids.find(id) == required_tile_ids.end()) {
                    continue;
                }

                int x = tile_node["x"].as<int>() * tile_size;
                int y = tile_node["y"].as<int>() * tile_size;
                Vector2D position(x, y);
                tile_sheet.tiles.emplace(id, position);
            }

            // Only add the sheet if it contains any required tiles
            if (!tile_sheet.tiles.empty()) {
                sheets.push_back(tile_sheet);
            }
        }
    }

    void load_into(SdlTile& tile) {
        for (const auto& sheet: sheets) {
            tile.add_sheet(sheet);
        }
    }
};

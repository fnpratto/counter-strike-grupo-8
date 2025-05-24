#include "map_view.h"

#include <stdexcept>

#include <yaml-cpp/yaml.h>

const std::string& BACKGROUND2_PATH = "../assets/gfx/tiles/dust.bmp";
const std::string& TILES_PATH = "../assets/gfx/backgrounds/stone1.jpg";
const std::string& CHARACTER_PATH = "../assets/gfx/player/ct2.xcf";
const std::string& MAP_PATH = "../map_view/map_50x50.yaml";
const int tile_width = 32;
const int tile_height = 32;

Map::Map(SdlWindow& window):
        window(window),
        background(BACKGROUND2_PATH, window),
        tiles(TILES_PATH, window),
        character(CHARACTER_PATH, window),
        DISPLAY_WIDTH(window.getWidth()),
        DISPLAY_HEIGHT(window.getHeight()) {}

void Map::load_map(int /*map_id*/) {
    YAML::Node map_data = YAML::LoadFile(MAP_PATH);

    if (!map_data["tiles"] || !map_data["tiles"]["floors"]) {
        throw std::runtime_error("Invalid or missing 'tiles.floors' in YAML.");
    }

    this->map_data = map_data;  // Store it for later use in `build()`
    build();
}

void Map::build() {
    YAML::Node tiles = map_data["tiles"];
    if (tiles["floors"]) {
        for (const auto& tile_data: tiles["floors"]) {
            int x = tile_data["x"].as<int>();
            int y = tile_data["y"].as<int>();
            int id = tile_data["id"].as<int>();

            // TODO: store or render based on x, y, id
            // Source area in tileset: shift horizontally by id * tile_width
            Area src(id * tile_width, 0, tile_width, tile_height);

            // Destination position on screen: place tile at x,y grid
            Area dest(x * tile_width, y * tile_height, tile_width, tile_height);

            // Render this tile from tiles texture to the screen
            tiles.render(src, dest);
        }
    }
}


void Map::render() {
    Area src(0, 0, 28, 29);
    Area iconDest(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, 30, 10);

    character.render(src, iconDest);
}

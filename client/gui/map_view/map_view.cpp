#include "map_view.h"

#include <stdexcept>


const std::string& BACKGROUND2_PATH = "../assets/gfx/tiles/dust.bmp";
const std::string& TILES_PATH = "../assets/gfx/tiles/default_aztec.png";
const std::string& CHARACTER_PATH = "../assets/gfx/player/ct2.xcf";
const std::string& MAP_PATH = "../client/gui/map_view/map_aztec.yaml";
const int tile_width = 32;
const int tile_height = 32;

const int tile_size = 64;

Map::Map(SdlWindow& window):
        window(window),
        background(BACKGROUND2_PATH, window),
        tiles_area(TILES_PATH, window),
        character(CHARACTER_PATH, window),
        DISPLAY_WIDTH(window.getWidth()),
        DISPLAY_HEIGHT(window.getHeight()) {}

void Map::load_map(int map_id) {

    map_data = YAML::LoadFile(MAP_PATH);
    build();
}

void Map::build() {
    YAML::Node tiles = map_data["tiles"];
    if (tiles["floors"]) {
        for (const auto& tile_data: tiles["floors"]) {
            int x = tile_data["x"].as<int>();
            int y = tile_data["y"].as<int>();
            int id = tile_data["id"].as<int>();
            int id_x = id % 5;  // Calculate the x position in the 5x10 grid
            int id_y = id / 5;  // Calculate the y position in the 5x10 grid

            // TODO: store or render based on x, y, id
            // Source area in tileset: shift horizontally by id * tile_width

            Area src(id_x * tile_width, tile_height * id_y, tile_width, tile_height);

            // Destination position on screen: place tile at x,y grid
            Area dest(x * tile_size, y * tile_size, tile_size, tile_size);

            // Render this tile from tiles texture to the screen
            tiles_area.render(src, dest);
        }
    }
}


void Map::render() {
    Area src(0, 0, 28, 29);
    Area iconDest(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, 28, 29);
    load_map(0);  // Load the map with ID 0
    character.render(src, iconDest);
}

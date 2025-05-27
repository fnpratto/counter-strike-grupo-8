#include "map_view.h"
// Placeholder: adjust these paths as needed
// #define MAPS_FILE "assets/maps/maps.yaml"
const std::string& BACKGROUND2_PATH = "../assets/gfx/tiles/dust.bmp";
const std::string& TILES_PATH = "../assets/gfx/backgrounds/stone1.jpg";
const std::string& CHARACTER_PATH = "../assets/gfx/player/ct2.xcf";


Map::Map(SdlWindow& window):
        window(window),
        background(BACKGROUND2_PATH, window),
        character(CHARACTER_PATH, window),
        tiles(TILES_PATH, window),
        DISPLAY_WIDTH(window.getWidth()),
        DISPLAY_HEIGHT(window.getHeight()) {}

void Map::load_map(/*int map_id*/) {


    /*YAML::Node maps_yaml = YAML::LoadFile(MAPS_FILE);

    if (!maps_yaml["maps"] || !maps_yaml["maps"][map_id]) {
        throw std::runtime_error("Invalid map ID or missing map entry in YAML.");
    }

    std::string texture_path = maps_yaml["maps"][map_id]["texture"].as<std::string>();
    std::string yaml_path = maps_yaml["maps"][map_id]["yaml"].as<std::string>();

    try {
        background.load_from_file(texture_path);
        tiles.load_from_file(yaml_path);  // Assuming tiles could be another image
    } catch (const std::exception& e) {
        std::cerr << "Error loading map resources: " << e.what() << std::endl;
    }*/
}


void Map::render() {

    Area src(0, 0, 28, 29);
    Area iconDest(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, 30, 10);

    character.render(src, iconDest);
}

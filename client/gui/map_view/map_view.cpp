#include "map_view.h"

#include <stdexcept>


const std::string& BACKGROUND2_PATH = "../assets/gfx/tiles/dust.bmp";
const std::string& TILES_PATH = "../assets/gfx/tiles/default_aztec.png";
const std::string& CHARACTER_PATH = "../assets/gfx/player/ct2.xcf";
const std::string& MAP_PATH = "../client/gui/map_view/cod_big_map_20x20.yaml";

const int tile_width = 32;
const int tile_height = 32;

const int tile_size = 64;


Map::Map(SdlWindow& window):
        window(window),
        background(BACKGROUND2_PATH, window),
        tiles_area(TILES_PATH, window),
        character(CHARACTER_PATH, window),
        DISPLAY_WIDTH(window.getWidth()),
        DISPLAY_HEIGHT(window.getHeight()) {
    character_x = 300;
    character_y = 300;
    // WALK UP (top-left)
    walkUpClips[0] = {0, 0, 32, 32};
    walkUpClips[1] = {32, 0, 32, 32};
    walkUpClips[2] = {0, 32, 32, 32};
    walkUpClips[3] = {32, 32, 32, 32};
    walkUpClips[4] = {0, 64, 32, 32};
    walkUpClips[5] = {32, 64, 32, 32};

    // WALK DOWN (top-right)
    walkDownClips[0] = {64, 0, 32, 32};
    walkDownClips[1] = {96, 0, 32, 32};
    walkDownClips[2] = {64, 32, 32, 32};
    walkDownClips[3] = {96, 32, 32, 32};
    walkDownClips[4] = {64, 64, 32, 32};
    walkDownClips[5] = {96, 64, 32, 32};

    // WALK RIGHT (bottom-left)
    walkRightClips[0] = {0, 96, 32, 32};
    walkRightClips[1] = {32, 96, 32, 32};
    walkRightClips[2] = {0, 128, 32, 32};
    walkRightClips[3] = {32, 128, 32, 32};
    walkRightClips[4] = {0, 160, 32, 32};
    walkRightClips[5] = {32, 160, 32, 32};

    // WALK LEFT (bottom-right)
    walkLeftClips[0] = {64, 96, 32, 32};
    walkLeftClips[1] = {96, 96, 32, 32};
    walkLeftClips[2] = {64, 128, 32, 32};
    walkLeftClips[3] = {96, 128, 32, 32};
    walkLeftClips[4] = {64, 160, 32, 32};
    walkLeftClips[5] = {96, 160, 32, 32};
}

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

void Map::update_character(int x, int y, Direction dir) {
    character_x = x;
    character_y = y;
    current_direction = dir;

    // Advance animation frame
    animation_frame = (animation_frame + 1) % 6;
}


void Map::render() {
    const SDL_Rect* currentClip = nullptr;
    switch (current_direction) {
        case DIR_DOWN:
            currentClip = &walkDownClips[animation_frame];
            break;
        case DIR_UP:
            currentClip = &walkUpClips[animation_frame];
            break;
        case DIR_LEFT:
            currentClip = &walkLeftClips[animation_frame];
            break;
        case DIR_RIGHT:
            currentClip = &walkRightClips[animation_frame];
            break;
    }
    // Replace static src with animated clip
    Area iconSrc(currentClip->x, currentClip->y, currentClip->w, currentClip->h);
    Area iconDest(character_x, character_y, currentClip->w, currentClip->h);

    load_map(0);  // Load the map with ID 0
    /*for*/
    character.render(iconSrc, iconDest);
}

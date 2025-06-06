#include "map_view.h"

#include <stdexcept>


const std::string& BACKGROUND2_PATH = "../assets/gfx/tiles/dust.bmp";
const std::string& TILES_PATH = "../assets/gfx/tiles/default_aztec.png";
const std::string& CHARACTER_PATH = "../assets/gfx/player/ct2.xcf";
const std::string& MAP_PATH = "../client/gui/map_view/cod_big_map_20x20-1.yaml";

const int tile_width = 32;
const int tile_height = 32;
const int tile_size = 96;


Map::Map(SdlWindow& window):
        window(window),
        background(BACKGROUND2_PATH, window),
        tiles_area(TILES_PATH, window),
        character(CHARACTER_PATH, window),
        yaml_map_data(YAML::LoadFile(MAP_PATH)),
        DISPLAY_WIDTH(window.getWidth()),
        DISPLAY_HEIGHT(window.getHeight()),
        map_data({Vector2D(100, 100)}) {
    character_x = -1;
    character_y = -1;
    current_direction = Direction::DIR_DOWN;
    currentClip = nullptr;
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

void Map::load_map(/*int map_id*/) { build(); }

void Map::build() {
    YAML::Node tiles = yaml_map_data["tiles"];
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

void Map::update_character(int x, int y) {
    if (character_x == -1 && character_y == -1) {
        character_x = x;
        character_y = y;
        return;
    }

    if (x > character_x && y > character_y) {
        current_direction = Direction::DIR_DOWN_RIGHT;
    } else if (x > character_x && y < character_y) {
        current_direction = Direction::DIR_UP_RIGHT;
    } else if (x < character_x && y > character_y) {
        current_direction = Direction::DIR_DOWN_LEFT;
    } else if (x < character_x && y < character_y) {
        current_direction = Direction::DIR_UP_LEFT;
    } else if (x > character_x) {
        current_direction = Direction::DIR_RIGHT;
    } else if (x < character_x) {
        current_direction = Direction::DIR_LEFT;
    } else if (y > character_y) {
        current_direction = Direction::DIR_DOWN;
    } else if (y < character_y) {
        current_direction = Direction::DIR_UP;
    }

    // Advance animation frame
    animation_frame = (animation_frame + 1) % 6;
    character_x = x;
    character_y = y;
}

void Map::update(GameUpdate state) {
    update_character(map_data.position.get_x(), map_data.position.get_y());
    map_data.position = state.get_players().at("Player1").get_pos();
    std::cout << "Map position updated to: (" << map_data.position.get_x() << ", "
              << map_data.position.get_y() << ")" << std::endl;
}


void Map::render() {

    /*switch (current_direction) {  // todo diagonales
        case Direction::DIR_DOWN:
            currentClip = &walkDownClips[animation_frame];
            break;
        case Direction::DIR_UP:
            currentClip = &walkUpClips[animation_frame];
            break;
        case Direction::DIR_UP_RIGHT:
        case Direction::DIR_DOWN_RIGHT:
        case Direction::DIR_DOWN_LEFT:
        case Direction::DIR_UP_LEFT:
            // Handle diagonal directions (e.g., use walkRightClips or walkLeftClips as fallback)
            currentClip = &walkRightClips[animation_frame];  // Example fallback
            break;
            break;
        case Direction::DIR_LEFT:
            currentClip = &walkLeftClips[animation_frame];
            break;
        case Direction::DIR_RIGHT:
            currentClip = &walkRightClips[animation_frame];
            break;
    }
    // Replace static src with animated clip
    Area iconSrc(currentClip->x, currentClip->y, currentClip->w, currentClip->h);
    Area iconDest(map_data.position.get_x(), map_data.position.get_y(), currentClip->w * 1.5,
                  currentClip->h * 1.5);*/

    load_map();  // Load the map with ID 0
                 /*for*/
                 // character.render(iconSrc, iconDest);
                 //  Area src(0, 0, 28, 29);
                 //  Area iconDest(map_data.position.get_x(), map_data.position.get_y(), 50, 50);
                 // std::cout << "Rendering map at position: (" << map_data.position.get_x() << ", "
                 // << map_data.position.get_y() << ")" << std::endl;

    // character.render(src, iconDest);
}

#include "map_view.h"

#include <stdexcept>


const std::string& BACKGROUND2_PATH = "../assets/gfx/tiles/dust.bmp";
const std::string& TILES_PATH = "../assets/gfx/tiles/default_aztec.png";
const std::string& CHARACTER_PATH = "../assets/gfx/player/ct2.xcf";
const std::string& MAP_PATH = "../client/gui/map_view/cod_big_map_20x20-1.yaml";


const int tile_width = 32;
const int tile_height = 32;
const int tile_size = 96;
const int total_map_width = 640;   // TODO
const int total_map_height = 640;  // TODO


Map::Map(SdlWindow& window, const std::string& name, const GameUpdate& state):
        window(window),
        background(BACKGROUND2_PATH, window),
        tiles_area(TILES_PATH, window),
        character(CHARACTER_PATH, window),
        yaml_map_data(YAML::LoadFile(MAP_PATH)),
        DISPLAY_WIDTH(window.getWidth()),
        DISPLAY_HEIGHT(window.getHeight()),
        camera(DISPLAY_WIDTH, DISPLAY_HEIGHT),  // Define total map size in pixels,
        player_name(name),
        game_state(state) {

    // TODO load_map(0); segun id del yaml
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

    Vector2D offset = camera.get_offset();
    camera_x_px = offset.get_x() * tile_size;
    camera_y_px = offset.get_y() * tile_size;

    camera_x_tile = camera_x_px / tile_size;
    camera_y_tile = camera_y_px / tile_size;

    tiles_in_view_x = DISPLAY_WIDTH / tile_size + 2;  // +2 for margin/padding
    tiles_in_view_y = DISPLAY_HEIGHT / tile_size + 2;
}

void Map::render_map() {
    YAML::Node tiles = yaml_map_data["tiles"];
    if (tiles["floors"]) {
        for (const auto& tile_data: tiles["floors"]) {
            int x = tile_data["x"].as<int>();
            int y = tile_data["y"].as<int>();

            if (x < camera_x_tile || x > camera_x_tile + tiles_in_view_x || y < camera_y_tile ||
                y > camera_y_tile + tiles_in_view_y) {
                continue;
            }

            int id = tile_data["id"].as<int>();
            int id_x = id % 5;  // Calculate the x position in the 5x10 grid
            int id_y = id / 5;  // Calculate the y position in the 5x10 grid


            Area screen_dest((x * tile_size) - camera.get_offset().get_x(),
                             (y * tile_size) - camera.get_offset().get_y(), tile_size, tile_size);

            Area src(id_x * tile_width, tile_height * id_y, tile_width, tile_height);

            // Render this tile from tiles texture to the screen
            tiles_area.render(src, screen_dest);
        }
    }
}

void Map::update_character_direction() {
    int x = game_state.get_players().at(player_name).get_pos().get_x();
    int y = game_state.get_players().at(player_name).get_pos().get_y();

    if (x > character_x && y > character_y) {
        current_direction = Direction::DIR_DOWN_RIGHT;
        animation_frame = (animation_frame + 1) % 6;
    } else if (x > character_x && y < character_y) {
        current_direction = Direction::DIR_UP_RIGHT;
        animation_frame = (animation_frame + 1) % 6;
    } else if (x < character_x && y > character_y) {
        current_direction = Direction::DIR_DOWN_LEFT;
        animation_frame = (animation_frame + 1) % 6;
    } else if (x < character_x && y < character_y) {
        current_direction = Direction::DIR_UP_LEFT;
        animation_frame = (animation_frame + 1) % 6;
    } else if (x > character_x) {
        current_direction = Direction::DIR_RIGHT;
        animation_frame = (animation_frame + 1) % 6;
    } else if (x < character_x) {
        current_direction = Direction::DIR_LEFT;
        animation_frame = (animation_frame + 1) % 6;
    } else if (y > character_y) {
        current_direction = Direction::DIR_DOWN;
        animation_frame = (animation_frame + 1) % 6;
    } else if (y < character_y) {
        current_direction = Direction::DIR_UP;
        animation_frame = (animation_frame + 1) % 6;
    }

    character_x = x;
    character_y = y;
}


void Map::render() {
    update_character_direction();
    switch (current_direction) {  // todo diagonales
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
            currentClip = &walkRightClips[animation_frame];  // TODO
            break;
        case Direction::DIR_LEFT:
            currentClip = &walkLeftClips[animation_frame];
            break;
        case Direction::DIR_RIGHT:
            currentClip = &walkRightClips[animation_frame];
            break;
    }

    camera.center(game_state.get_players().at(player_name).get_pos());
    render_map();

    Area iconSrc(currentClip->x, currentClip->y, currentClip->w, currentClip->h);
    Area iconDest(character_x - camera.get_offset().get_x(),
                  character_y - camera.get_offset().get_y(), currentClip->w * 1.5,
                  currentClip->h * 1.5);
    std::cout << "Rendering character at tile ("
              << game_state.get_players().at(player_name).get_pos().get_x() -
                         camera.get_offset().get_x()
              << ", "
              << game_state.get_players().at(player_name).get_pos().get_y() -
                         camera.get_offset().get_y()
              << ")" << std::endl;
    std::cout << "Rendering character at tile (" << character_x - camera.get_offset().get_x()
              << ", " << character_y - camera.get_offset().get_y() << ")" << std::endl;

    // TODO RENDERIZAR JUGADORES QUE ESTEN A LA VISTA

    character.render(iconSrc, iconDest);
}

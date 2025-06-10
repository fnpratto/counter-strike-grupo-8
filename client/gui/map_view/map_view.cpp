#include "map_view.h"

#include <stdexcept>


const std::string& BACKGROUND2_PATH = "../assets/gfx/tiles/dust.bmp";
const std::string& TILES_PATH = "../assets/gfx/tiles/default_aztec.png";
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
        yaml_map_data(YAML::LoadFile(MAP_PATH)),
        DISPLAY_WIDTH(window.getWidth()),
        DISPLAY_HEIGHT(window.getHeight()),
        camera(DISPLAY_WIDTH, DISPLAY_HEIGHT),  // Define total map size in pixels,
        player_name(name),
        game_state(state) {

    // TODO load_map(0); segun id del yaml


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

void Map::render() {
    camera.center(game_state.get_players().at(player_name).get_pos());
    render_map();

    // Render other players
    for (const auto& [name, player]:  // cppcheck-suppress[unassignedVariable]
         game_state.get_players()) {
        auto current_clip = characters[name].get_current_clip();

        Area otherPlayerSrc(current_clip.x, current_clip.y, current_clip.w, current_clip.h);
        Area otherPlayerDest(player.get_pos().get_x() - camera.get_offset().get_x(),
                             player.get_pos().get_y() - camera.get_offset().get_y(),
                             current_clip.w * 1.5, current_clip.h * 1.5);

        character.render(otherPlayerSrc, otherPlayerDest);
        character.render(player.get_pos().get_x() - camera.get_offset().get_x(),
                         player.get_pos().get_y() - camera.get_offset().get_y(), &current_clip, 0.0,
                         nullptr, SDL_FLIP_NONE);
    }
}

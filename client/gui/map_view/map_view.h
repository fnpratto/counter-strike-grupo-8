#ifndef MAP_VIEW_H
#define MAP_VIEW_H

#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include "../window_elements/area.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/updates/game_update.h"

#include "camera.h"

enum Direction {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP_LEFT,
    DIR_UP_RIGHT,
    DIR_DOWN_LEFT,
    DIR_DOWN_RIGHT
};

class Map {
public:
    explicit Map(SdlWindow& window, const std::string& name, const GameUpdate& state);
    void render();


private:
    SdlWindow& window;
    SdlTexture background;
    SdlTexture tiles_area;
    SdlTexture character;
    YAML::Node yaml_map_data;
    const int DISPLAY_WIDTH;
    const int DISPLAY_HEIGHT;
    int character_x;
    int character_y;
    Direction current_direction;
    int animation_frame = 0;
    SDL_Rect walkDownClips[6];
    SDL_Rect walkUpClips[6];
    SDL_Rect walkLeftClips[6];
    SDL_Rect walkRightClips[6];
    SDL_Rect* currentClip;
    Camera camera;
    const std::string& player_name;
    const GameUpdate& game_state;
    void update_character_direction();
    void render_map();
    int camera_x_px;
    int camera_y_px;

    int camera_x_tile;
    int camera_y_tile;

    int tiles_in_view_x;  // +2 for margin/padding
    int tiles_in_view_y;
};

#endif  // MAP_VIEW_H

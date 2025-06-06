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

struct MapData {
    Vector2D position;
};

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
    explicit Map(SdlWindow& window);

    void render();

    void update(GameUpdate state);

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
    MapData map_data;
    SDL_Rect* currentClip;
    Camera camera;
    void update_character(int x, int y /*, Direction dir*/);
    void render_map();
};

#endif  // MAP_VIEW_H

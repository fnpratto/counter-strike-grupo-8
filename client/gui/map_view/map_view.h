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
// #include "camera.h"

class Map {
public:
    explicit Map(SdlWindow& window);

    void load_map(int map_id);

    void render();
    void update_character(int x, int y);

private:
    SdlWindow& window;
    SdlTexture background;
    SdlTexture tiles_area;
    SdlTexture character;
    YAML::Node map_data;
    const int DISPLAY_WIDTH;
    const int DISPLAY_HEIGHT;
    int character_x;
    int character_y;
    void build();
};

#endif  // MAP_VIEW_H

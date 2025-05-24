#ifndef MAP_VIEW_H
#define MAP_VIEW_H

#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include "../window_elements/area.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"

// #include "camera.h"

class Map {
public:
    explicit Map(SdlWindow& window);

    void load_map(int map_id);

    void render();

private:
    SdlWindow& window;
    SdlTexture background;
    SdlTexture character;
    SdlTexture tiles;
    int DISPLAY_WIDTH;
    int DISPLAY_HEIGHT;
};

#endif  // MAP_VIEW_H

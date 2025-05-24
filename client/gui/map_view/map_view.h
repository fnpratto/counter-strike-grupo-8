#ifndef MAP_VIEW_H
#define MAP_VIEW_H

#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include "../window_elements/Area.h"
#include "../window_elements/SdlTexture.h"
#include "../window_elements/SdlWindow.h"

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
};

#endif  // MAP_VIEW_H

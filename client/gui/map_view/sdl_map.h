#ifndef MAP_VIEW_H
#define MAP_VIEW_H

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include "../window_elements/area.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/map/map.h"
#include "common/updates/game_update.h"

#include "sdl_camera.h"
#include "sdl_tile.h"

class SdlMap {
    SdlWindow& window;
    const SdlCamera& camera;

    Map map;

    SdlTile tile;

public:
    explicit SdlMap(SdlWindow& window, const SdlCamera& camera, Map&& map);
    void render();
};

#endif  // MAP_VIEW_H

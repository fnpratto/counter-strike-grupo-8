#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>

#include "../window_elements/area.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "client/gui/window_elements/sdl_animation.h"
#include "common/updates/game_update.h"

#include "sdl_camera.h"
#include "tile_sheet.h"


class SdlTile {
    const SdlWindow& window;
    const SdlCamera& camera;

    static constexpr int WIDTH = 32;
    static constexpr int HEIGHT = 32;

    std::map<int, std::pair<std::shared_ptr<SdlTexture>, Vector2D>> tiles;

public:
    explicit SdlTile(const SdlWindow& window, const SdlCamera& camera);

    void add_sheet(const TileSheet& sheet);

    void render(const Tile& tile);
};

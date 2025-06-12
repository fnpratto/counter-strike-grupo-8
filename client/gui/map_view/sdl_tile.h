#pragma once

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
#include "client/gui/window_elements/sdl_animation.h"
#include "common/updates/game_update.h"

#include "sdl_camera.h"


class SdlTile {
    SdlWindow& window;
    const SdlCamera& camera;

    static constexpr const char* TILES_PATH = "../assets/gfx/tiles/default_aztec.png";
    static constexpr int WIDTH = 32;
    static constexpr int HEIGHT = 32;

    SdlTexture sheet;

public:
    explicit SdlTile(SdlWindow& window, const SdlCamera& camera):
            window(window), camera(camera), sheet(TILES_PATH, window, WIDTH, HEIGHT) {}

    template <typename T>
    void render(const T& tile) {
        auto position_from_cam = camera.get_screen_pos(tile.get_pos());

        // TODO : Implement proper tile rendering based on tile type
        SDL_Rect src_rect = {WIDTH, 0, WIDTH, HEIGHT};

        sheet.render(position_from_cam.get_x(), position_from_cam.get_y(), &src_rect);
    }
};

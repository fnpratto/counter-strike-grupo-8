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
#include "client/gui/animation.h"
#include "common/updates/game_update.h"

#include "camera.h"

const std::string& CHARACTER_PATH = "../assets/gfx/player/ct2.xcf";
class Character {
    static constexpr int WIDTH = 32;
    static constexpr int HEIGHT = 32;
    SdlWindow& window;

    Animation walk_animation;

public:
    explicit Character(SdlWindow& window):
            window(window),
            walk_animation(SdlTexture(CHARACTER_PATH, window, WIDTH, HEIGHT),
                           // TODO This could be changed with a strip of sprites instead of this
                           // mosaic and simply iterated over x
                           std::vector<SDL_Rect>({{0, 0, WIDTH, HEIGHT},
                                                  {32, 0, WIDTH, HEIGHT},
                                                  {0, 32, WIDTH, HEIGHT},
                                                  {32, 32, WIDTH, HEIGHT},
                                                  {0, 64, WIDTH, HEIGHT},
                                                  {32, 64, WIDTH, HEIGHT}})) {}

    void reset() { walk_animation.reset(); }

    void render(const PlayerUpdate& state) {
        walk_animation.render(state.get_pos().get_x() - state.get_camera_offset().get_x(),
                              state.get_pos().get_y() - state.get_camera_offset().get_y(),
                              state.get_angle());
    }
};

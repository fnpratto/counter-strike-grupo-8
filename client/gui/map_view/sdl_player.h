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

class SdlPlayer {
    SdlWindow& window;
    const SdlCamera& camera;

    static constexpr const char* CHARACTER_PATH = "../assets/gfx/player/ct2.xcf";
    static constexpr int WIDTH = 32;
    static constexpr int HEIGHT = 32;

    SdlAnimation walk_animation;

public:
    explicit SdlPlayer(SdlWindow& w, const SdlCamera& camera);

    void render(const PlayerUpdate& state);
};

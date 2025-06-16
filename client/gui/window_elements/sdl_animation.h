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
#include "common/updates/game_update.h"

class SdlAnimation {
    SdlTexture texture;
    std::vector<SDL_Rect> clips;
    int animation_frame;

public:
    SdlAnimation(const SdlWindow& window, const std::string& path,
                 const std::vector<SDL_Rect>& clips);

    void reset();

    // TODO animations should be independent of the framerate
    void render(int x, int y, double angle);
};

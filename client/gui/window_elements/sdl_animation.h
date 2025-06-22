#pragma once

#include <chrono>
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

#include "sdl_texture.h"
#include "sdl_window.h"

class SdlAnimation {
    SdlTexture texture;
    std::vector<SDL_Rect> clips;

    // Time-based animation properties
    std::chrono::steady_clock::time_point start_time;
    std::chrono::milliseconds animation_duration;
    bool repeats;
    bool finished;

public:
    SdlAnimation(const SdlWindow& window, const std::string& path,
                 const std::vector<SDL_Rect>& clips, std::chrono::milliseconds duration,
                 bool repeats = true, float scale = 1.0f);

    void render(int x, int y, double angle);
    bool is_finished() const;
    void reset();
};

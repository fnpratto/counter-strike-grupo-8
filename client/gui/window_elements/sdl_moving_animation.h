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
#include "common/utils/vector_2d.h"

#include "sdl_texture.h"
#include "sdl_window.h"

class SdlMovingAnimation {
    std::unique_ptr<SdlTexture> texture;

    // Time-based animation properties
    std::chrono::steady_clock::time_point start_time;
    std::chrono::milliseconds animation_duration;
    bool repeats;
    bool finished;
    float progress;

public:
    SdlMovingAnimation(const SdlWindow& window, const std::string& path, int width, int height,
                       std::chrono::milliseconds duration, bool repeats = false);

    void render(const Vector2D& origin, const Vector2D& destination, double angle = 0);
    bool is_finished() const;
    void reset();
    float get_progress() const;
};

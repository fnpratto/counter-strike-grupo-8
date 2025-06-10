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

#include "camera.h"

class SdlAnimation {
    SdlTexture texture;
    std::vector<SDL_Rect> clips;
    int animation_frame = 0;

public:
    SdlAnimation(const SdlTexture& texture, const std::vector<SDL_Rect>& clips):
            texture(texture), clips(clips) {}

    void reset() { animation_frame = 0; }

    void render(int x, int y, double angle = 0) {
        texture.render(x, y, &clips[animation_frame], angle, nullptr, SDL_FLIP_NONE);
        animation_frame = (animation_frame + 1) % clips.size();
    }
};

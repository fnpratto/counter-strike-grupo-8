#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include "../map_view/sdl_gun.h"
#include "../window_elements/area.h"
#include "../window_elements/sdl_animation.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "client/gui/window_elements/sdl_animation.h"
#include "common/updates/game_update.h"

#include "sdl_camera.h"

class SdlPlayer {
    SdlWindow& window;
    const SdlCamera& camera;
    SdlAnimation walk_animation;
    const GameUpdate& game_state;
    std::string playerName;
    SDLGun weapon;

    static constexpr const char* CHARACTER_PATH = "../assets/gfx/player/ct2.xcf";
    static constexpr const char* WALKING_ANIMATION = "../assets/gfx/player/legs_animation.xcf";
    static constexpr int WIDTH = 32;
    static constexpr int HEIGHT = 32;


public:
    explicit SdlPlayer(SdlWindow& w, const SdlCamera& camera, const GameUpdate& game_state,
                       const std::string& playerName);

    void render(const PlayerUpdate& state);
};

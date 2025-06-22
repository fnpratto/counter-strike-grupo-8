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
#include "client/gui/window_elements/area.h"
#include "client/gui/window_elements/sdl_animation.h"
#include "client/gui/window_elements/sdl_texture.h"
#include "client/gui/window_elements/sdl_window.h"
#include "common/map/map.h"
#include "common/responses.h"
#include "common/updates/game_update.h"
#include "common/utils/vector_2d.h"

#include "sdl_camera.h"
#include "sdl_map.h"
#include "sdl_player.h"


class SdlKnifeSlash {
    static constexpr const int WIDTH = 32;
    static constexpr const int HEIGHT = 32;
    static constexpr const float SCALE = 1.3f;
    static constexpr int x_offset = -(static_cast<int>(WIDTH * SCALE) / 2);
    static constexpr int y_offset = -(static_cast<int>(HEIGHT * SCALE) / 2);

    const SdlCamera& camera;

    Vector2D origin;
    HitResponse hit_response;

    SdlAnimation slash_animation;

public:
    explicit SdlKnifeSlash(const SdlWindow& w, const SdlCamera& camera, Vector2D origin,
                           const HitResponse& hit_response);

    void render();
    bool is_finished() const;
};

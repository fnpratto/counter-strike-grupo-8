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
#include "client/gui/window_elements/sdl_moving_animation.h"
#include "client/gui/window_elements/sdl_texture.h"
#include "client/gui/window_elements/sdl_window.h"
#include "common/map/map.h"
#include "common/responses.h"
#include "common/updates/game_update.h"

#include "sdl_camera.h"
#include "sdl_map.h"
#include "sdl_player.h"


class SdlBullet {
    const SdlCamera& camera;

    Vector2D origin;
    HitResponse hit_response;
    float angle;

    SdlMovingAnimation bullet_animation;

public:
    explicit SdlBullet(const SdlWindow& w, const SdlCamera& camera, Vector2D origin,
                       const HitResponse& hit_response);

    void render();
    bool is_finished() const;

private:
    void render_laser(const Vector2D& origin, float length, float angle);
    void render_hit_effect(const Vector2D& pos, float angle);
};

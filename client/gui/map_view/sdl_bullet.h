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
#include "client/gui/window_elements/sdl_texture.h"
#include "client/gui/window_elements/sdl_window.h"
#include "common/map/map.h"
#include "common/updates/game_update.h"

#include "sdl_camera.h"
#include "sdl_map.h"
#include "sdl_player.h"


class SdlBullet {
private:
    SdlWindow& window;
    SdlTexture laser;
    SdlTexture hit_effect;
    void render_laser(const Vector2D& origin, float length, float angle);
    void render_hit_effect(const Vector2D& pos, float angle);


public:
    explicit SdlBullet(SdlWindow& w);

    void render(Vector2D get_origin, Vector2D get_hit_pos, Vector2D get_hit_dir, bool is_hit,
                bool is_knife);
};

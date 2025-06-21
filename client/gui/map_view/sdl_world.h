#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include "../window_elements/area.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/responses.h"
#include "common/updates/game_update.h"

#include "sdl_bullet.h"
#include "sdl_camera.h"
#include "sdl_map.h"
#include "sdl_player.h"

struct BulletInfo {
    Vector2D origin;
    Vector2D hit_pos;
    Vector2D hit_dir;
    bool is_hit;
    bool is_knife;
    // int range_item; TODO
};

class SdlWorld {
    static constexpr const char* BACKGROUND_PATH = "../assets/gfx/tiles/dust.bmp";

    SdlWindow& window;
    const GameUpdate& game_state;

    const std::string& player_name;
    SdlCamera camera;
    SdlMap map;
    std::unique_ptr<SdlPlayer> player;

    // SdlTexture background;  // TODO: Load a background texture

public:
    SdlWorld(SdlWindow& window, Map&& map, const GameUpdate& game_state,
             const std::string& player_name);

    void render();
    void handle_hit(HitResponse&& hit);
};

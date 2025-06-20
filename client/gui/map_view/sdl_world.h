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
#include "common/map/map.h"
#include "common/updates/game_update.h"

#include "sdl_bullet.h"
#include "sdl_camera.h"
#include "sdl_item.h"
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
private:
    static constexpr const char* BACKGROUND_PATH = "../assets/gfx/tiles/dust.bmp";
    SdlWindow& window;
    const GameUpdate& game_state;
    const std::string& player_name;
    SdlCamera camera;
    std::map<std::string, std::unique_ptr<SdlPlayer>> players;
    SdlMap map;
    SdlBullet bullet;
    std::vector<BulletInfo> bullets_info;
    SdlItem items;

    void addBulletInfo(const Vector2D& origin, const Vector2D& hit, const Vector2D& dir,
                       bool is_hit, bool is_melee);
    Map build_default_map();

    // SdlTexture background;  // TODO: Load a background texture

public:
    SdlWorld(SdlWindow& window, const GameUpdate& game_state, const std::string& player_name);

    void render();
    void handleHit(Vector2D get_origin, Vector2D get_hit_pos, Vector2D get_hit_dir, bool is_hit);
};

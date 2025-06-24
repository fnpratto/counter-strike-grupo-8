#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "client/game_config.h"
#include "client/gui/window_elements/sdl_texture.h"
#include "client/gui/window_elements/sdl_window.h"
#include "common/models.h"
#include "common/utils/vector_2d.h"

class SdlItem {

    static constexpr std::string_view AK47_PATH = "../assets/gfx/guns/ak47.xcf";
    static constexpr std::string_view M3_PATH = "../assets/gfx/guns/m3.xcf";
    static constexpr std::string_view GLOCK_PATH = "../assets/gfx/guns/glock.xcf";
    static constexpr std::string_view AWP_PATH = "../assets/gfx/guns/awp.xcf";
    static constexpr std::string_view KNIFE_PATH = "../assets/gfx/guns/knife.xcf";
    static constexpr std::string_view BOMB_PATH = "../assets/gfx/guns/bomb.xcf";

    const SdlWindow& window;
    std::unordered_map<GunType, std::unique_ptr<SdlTexture>> gun_textures;
    std::unique_ptr<SdlTexture> knife_texture;
    std::unique_ptr<SdlTexture> bomb_texture;


public:
    explicit SdlItem(const SdlWindow& window);
    void render_gun(GunType gun_type, Vector2D pos, float angle);
    void render_knife(Vector2D pos, float angle);
    void render_bomb(Vector2D pos, float angle);
};

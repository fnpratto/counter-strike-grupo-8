#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "client/game_config.h"
#include "client/gui/window_elements/sdl_texture.h"
#include "client/gui/window_elements/sdl_window.h"
#include "common/updates/game_update.h"

#include "sdl_camera.h"

class SdlWorldItem {
private:
    const SdlWindow& window;
    SdlTexture awp_t;
    SdlTexture m3_t;
    SdlTexture ak_t;
    SdlTexture bomb_t;
    SdlTexture bomb_plated;
    const GameUpdate& game_state;
    const SdlCamera& camera;

public:
    explicit SdlWorldItem(const SdlWindow& window, const GameUpdate& game_state,
                          const SdlCamera& camera);
    void render();
    void render_bomb();
    void render_dropped_guns();
};

#ifndef SDL_ITEM_H
#define SDL_ITEM_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "client/game_config.h"
#include "client/gui/window_elements/sdl_texture.h"
#include "client/gui/window_elements/sdl_window.h"
#include "common/updates/game_update.h"

#include "sdl_camera.h"

class SdlItem {
private:
    const SdlWindow& window;
    SdlTexture awp_t;
    SdlTexture m3_t;
    SdlTexture ak_t;
    SdlTexture bomb_t;
    const GameUpdate& game_state;
    const SdlCamera& camera;

public:
    explicit SdlItem(const SdlWindow& window, const GameUpdate& game_state,
                     const SdlCamera& camera);
    void render();
};

#endif  // SDL_ITEM_H

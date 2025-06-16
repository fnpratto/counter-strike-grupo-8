#ifndef SDL_GUN_H
#define SDL_GUN_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "client/game_config.h"
#include "client/gui/window_elements/sdl_texture.h"
#include "client/gui/window_elements/sdl_window.h"

class SDLGun {
private:
    SdlWindow& window;
    SdlTexture primaryTexture;
    SdlTexture secondaryTexture;
    SdlTexture meleeTexture;
    SdlTexture bombTexture;

public:
    explicit SDLGun(SdlWindow& window);
    void render(ItemSlot itemSlot, Area& dest, float angle);
};

#endif  // SDL_GUN_H

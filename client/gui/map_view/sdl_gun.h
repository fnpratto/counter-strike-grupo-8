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
    SDL_Texture primaryTexture;
    SDL_Texture secondaryTexture;
    SDL_Texture meleeTexture;
    SDL_Texture bombTexture;

public:
    explicit SDLGun(SdlWindow& window);
    ~SDLGun();

    void render(ItemSlot itemSlot, int x, int y, float angle);
};

#endif  // SDL_GUN_H

#include "sdl_gun.h"

#include "client/game_config.h"

// Constructor
SDLGun::SDLGun(SdlWindow& window):
        primaryTexture(GameConfig::Paths::PRIMARY_GUN_PATH, window, 32, 32),
        secondaryTexture(GameConfig::Paths::SECONDARY_GUN_PATH, window, 32, 32),
        meleeTexture(GameConfig::Paths::MELEE_GUN_PATH, window, 32, 32),
        bombTexture(GameConfig::Paths::BOMB_GUN_PATH, window, 32, 32) {}

// Destructor
SDLGun::~SDLGun() {
    // Note: Textures are managed externally, so no need to destroy them here.
}

// Render method
void SDLGun::render(ItemSlot itemSlot, int x, int y, float angle) {
    SDL_Texture* textureToRender = nullptr;

    switch (itemSlot) {
        case ItemSlot::Primary:
            textureToRender = primaryTexture;
            break;
        case ItemSlot::Secondary:
            textureToRender = secondaryTexture;
            break;
        case ItemSlot::Melee:
            textureToRender = meleeTexture;
            break;
        case ItemSlot::Bomb:
            textureToRender = bombTexture;

            break;
    }

    textureToRender.render(x, y, nullptr, 0.0f, nullptr, SDL_FLIP_NONE);
}

#include "sdl_gun.h"

#include "client/game_config.h"

// Constructor
SDLGun::SDLGun(SdlWindow& w):
        window(w),
        primaryTexture(std::string(GameConfig::Paths::PRIMARY_GUN_PATH), window, 64, 64),
        secondaryTexture(std::string(GameConfig::Paths::SECONDARY_GUN_PATH), window, 64, 64),
        meleeTexture(std::string(GameConfig::Paths::MELEE_GUN_PATH), window, 64, 64),
        bombTexture(std::string(GameConfig::Paths::BOMB_GUN_PATH), window, 64, 64) {}

// Render method
void SDLGun::render(ItemSlot itemSlot, Area& dest, float angle) {
    SDL_Rect src = {0, 0, 45, 45};
    switch (itemSlot) {
        case ItemSlot::Primary:
            primaryTexture.render(dest.getX(), dest.getY(), &src, angle, nullptr, SDL_FLIP_NONE);
            break;
        case ItemSlot::Secondary:
            secondaryTexture.render(dest.getX(), dest.getY(), &src, angle, nullptr, SDL_FLIP_NONE);
            break;
        case ItemSlot::Melee:
            meleeTexture.render(dest.getX(), dest.getY(), &src, angle, nullptr, SDL_FLIP_NONE);
            break;
        case ItemSlot::Bomb:
            bombTexture.render(dest.getX(), dest.getY(), &src, angle, nullptr, SDL_FLIP_NONE);
            break;
        default:
            std::cerr << "Error: Invalid item slot passed to SDLGun::render: "
                      << static_cast<int>(itemSlot) << std::endl;
            break;
    }
}

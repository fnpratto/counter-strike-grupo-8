#include "sdl_gun.h"

#include "client/game_config.h"

// Constructor
SDLGun::SDLGun(SdlWindow& w):
        window(w),
        primaryTexture(std::string(GameConfig::Paths::PRIMARY_GUN_PATH), window),
        secondaryTexture(std::string(GameConfig::Paths::SECONDARY_GUN_PATH), window),
        meleeTexture(std::string(GameConfig::Paths::MELEE_GUN_PATH), window),
        bombTexture(std::string(GameConfig::Paths::BOMB_GUN_PATH), window) {}

// Render method
void SDLGun::render(ItemSlot itemSlot, Area& dest, float angle) {
    std::cout << "Rendering gun for item slot: " << static_cast<int>(itemSlot) << std::endl;
    SDL_Rect src = {0, 0, 32, 32};
    switch (itemSlot) {
        case ItemSlot::Primary:
            primaryTexture.render(dest.getX() - 5, dest.getY() - 5, &src, angle, nullptr,
                                  SDL_FLIP_NONE);
            break;
        case ItemSlot::Secondary:
            secondaryTexture.render(dest.getX() - 5, dest.getY() - 5, &src, angle, nullptr,
                                    SDL_FLIP_NONE);
            break;
        case ItemSlot::Melee:
            meleeTexture.render(dest.getX() - 5, dest.getY() - 5, &src, angle, nullptr,
                                SDL_FLIP_NONE);
            break;
        case ItemSlot::Bomb:
            bombTexture.render(dest.getX() - 5, dest.getY() - 5, &src, angle, nullptr,
                               SDL_FLIP_NONE);
            break;
        default:
            std::cerr << "Error: Invalid item slot passed to SDLGun::render: "
                      << static_cast<int>(itemSlot) << std::endl;
            break;
    }
}

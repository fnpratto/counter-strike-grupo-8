#include "sdl_item.h"

#include <iostream>

#include "client/game_config.h"
#include "common/utils/vector_2d.h"

SdlItem::SdlItem(const SdlWindow& w): window(w) {
    gun_textures[GunType::AK47] =
            std::make_unique<SdlTexture>(std::string(AK47_PATH), window, 32, 32);
    gun_textures[GunType::M3] = std::make_unique<SdlTexture>(std::string(M3_PATH), window, 32, 32);
    gun_textures[GunType::Glock] =
            std::make_unique<SdlTexture>(std::string(GLOCK_PATH), window, 32, 32);
    gun_textures[GunType::AWP] =
            std::make_unique<SdlTexture>(std::string(AWP_PATH), window, 32, 32);

    knife_texture = std::make_unique<SdlTexture>(std::string(KNIFE_PATH), window, 32, 32);
    bomb_texture = std::make_unique<SdlTexture>(std::string(BOMB_PATH), window, 32, 32);
}

void SdlItem::render_gun(GunType gun_type, Vector2D pos, float angle) {
    auto it = gun_textures.find(gun_type);
    if (it != gun_textures.end()) {
        it->second->render(pos.get_x(), pos.get_y(), angle);
    } else {
        std::cerr << "Error: Invalid gun type passed to SdlItem::render_gun: "
                  << static_cast<int>(gun_type) << std::endl;
    }
}

void SdlItem::render_knife(Vector2D pos, float angle) {
    if (knife_texture) {
        knife_texture->render(pos.get_x(), pos.get_y(), angle);
    } else {
        std::cerr << "Error: Knife texture not loaded in SdlItem::render_knife." << std::endl;
    }
}

void SdlItem::render_bomb(Vector2D pos, float angle) {
    if (bomb_texture) {
        bomb_texture->render(pos.get_x(), pos.get_y(), angle);
    } else {
        std::cerr << "Error: Bomb texture not loaded in SdlItem::render_bomb." << std::endl;
    }
}

#include "sdl_player.h"

#include <chrono>
#include <cmath>

#include <SDL2/SDL.h>


SdlPlayer::SdlPlayer(const SdlWindow& window, const SdlCamera& cam):
        camera(cam),
        walk_animation(
                window, WALKING_ANIMATION,
                std::vector<SDL_Rect>(
                        {{0, 0, WIDTH, HEIGHT}, {32, 0, WIDTH, HEIGHT}, {64, 0, WIDTH, HEIGHT}}),
                std::chrono::milliseconds(600),  // 600ms for complete walk cycle
                true),                           // repeating animation
        item(window) {
    load_skins(window);
}

void SdlPlayer::render(const PlayerUpdate& player_state) {
    if (player_state.get_health() <= 0) {
        return;
    }
    if (player_state.get_velocity() == Vector2D(0, 0)) {
        walk_animation.reset();
    }

    Vector2D pos = get_pos(player_state);
    int x = pos.get_x();
    int y = pos.get_y();
    float angle = get_rotation(player_state);

    walk_animation.render(x, y, angle);  // feet

    ItemSlot item_slot = player_state.get_equipped_item();
    switch (item_slot) {
        case ItemSlot::Primary:
        case ItemSlot::Secondary: {
            GunUpdate gun = player_state.get_inventory().get_guns().at(item_slot);
            item.render_gun(gun.get_gun(), pos, angle);
            break;
        }
        case ItemSlot::Melee: {
            item.render_knife(pos, angle);
            break;
        }
        case ItemSlot::Bomb: {
            item.render_bomb(pos, angle);
            break;
        }
        default:
            break;
    }

    CharacterType character = player_state.get_character_type();
    auto& texture = character_textures.at(character);
    SDL_Rect clip{0, 0, WIDTH, HEIGHT};
    texture->render(x, y, &clip, angle);  // actual character
}

Vector2D SdlPlayer::get_pos(const PlayerUpdate& player_state) {
    return camera.get_screen_pos(player_state.get_pos()) + Vector2D(x_offset, y_offset);
}

float SdlPlayer::get_rotation(const PlayerUpdate& player_state) {
    auto aim_direction = player_state.get_aim_direction();
    float angle;
    if (aim_direction != Vector2D(0, 0)) {
        angle = std::atan2(aim_direction.get_y(), aim_direction.get_x()) * 180 / M_PI;
        angle += 90.0f;
    } else {
        angle = 0.0f;
    }
    return angle;
}

void SdlPlayer::load_skins(const SdlWindow& window) {
    character_textures[CharacterType::Seal_Force] =
            std::make_unique<SdlTexture>("../assets/gfx/player/ct1.bmp", window, WIDTH, HEIGHT);
    character_textures[CharacterType::German_GSG_9] =
            std::make_unique<SdlTexture>("../assets/gfx/player/ct2.bmp", window, WIDTH, HEIGHT);
    character_textures[CharacterType::UK_SAS] =
            std::make_unique<SdlTexture>("../assets/gfx/player/ct3.bmp", window, WIDTH, HEIGHT);
    character_textures[CharacterType::French_GIGN] =
            std::make_unique<SdlTexture>("../assets/gfx/player/ct4.bmp", window, WIDTH, HEIGHT);
    character_textures[CharacterType::Pheonix] =
            std::make_unique<SdlTexture>("../assets/gfx/player/t1.bmp", window, WIDTH, HEIGHT);
    character_textures[CharacterType::L337_Krew] =
            std::make_unique<SdlTexture>("../assets/gfx/player/t2.bmp", window, WIDTH, HEIGHT);
    character_textures[CharacterType::Arctic_Avenger] =
            std::make_unique<SdlTexture>("../assets/gfx/player/t3.bmp", window, WIDTH, HEIGHT);
    character_textures[CharacterType::Guerrilla] =
            std::make_unique<SdlTexture>("../assets/gfx/player/t4.bmp", window, WIDTH, HEIGHT);
}

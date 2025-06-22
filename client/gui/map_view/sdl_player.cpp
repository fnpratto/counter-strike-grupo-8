#include "sdl_player.h"

#include <chrono>
#include <cmath>

#include <SDL2/SDL.h>


SdlPlayer::SdlPlayer(SdlWindow& w, const SdlCamera& cam, const GameUpdate& game_state_param,
                     const std::string& player_name_param):
        window(w),
        camera(cam),
        walk_animation(
                w, WALKING_ANIMATION,
                std::vector<SDL_Rect>(
                        {{0, 0, WIDTH, HEIGHT}, {32, 0, WIDTH, HEIGHT}, {64, 0, WIDTH, HEIGHT}}),
                std::chrono::milliseconds(600),  // 600ms for complete walk cycle
                true),                           // repeating animation
        game_state(game_state_param),
        playerName(player_name_param),
        weapon(window) {
    load_skins();
}

void SdlPlayer::render() {
    const PlayerUpdate& state = game_state.get_players().at(playerName);
    if (state.get_health() <= 0) {
        return;
    }
    if (state.get_velocity() == Vector2D(0, 0)) {
        walk_animation.reset();
    }
    auto position_from_cam = camera.get_screen_pos(state.get_pos());
    auto aim_direction = state.get_aim_direction();
    float angle;
    if (aim_direction != Vector2D(0, 0)) {
        angle = std::atan2(aim_direction.get_y(), aim_direction.get_x()) * 180 / M_PI;
        angle += 90.0f;
    } else {
        angle = 0.0f;
    }

    render_skin(position_from_cam.get_x(), position_from_cam.get_y(), angle);
}


// TODO we shouldn't be loading all the skins and we shouldn't be checking which one to use every
// time
void SdlPlayer::render_skin(int x, int y, float angle) {
    static constexpr int x_offset = WIDTH / 2;
    static constexpr int y_offset = HEIGHT / 2;

    const PlayerUpdate& state = game_state.get_players().at(playerName);
    SDL_Rect clip{32, 32, WIDTH, HEIGHT};

    auto team = state.get_team();
    CharacterType type;

    try {
        type = state.get_character_type();
    } catch (const std::exception& e) {
        type = (team == Team::CT) ? CharacterType::Seal_Force : CharacterType::Pheonix;
    }

    auto& skins = (team == Team::CT) ? ct_skins : tt_skins;
    auto& texture = skins.at(type);

    walk_animation.render(x - x_offset, y - y_offset, angle);
    texture->render(x - x_offset, y - y_offset, &clip, angle, nullptr, SDL_FLIP_NONE);
    Area dest(x - x_offset - 5, y - y_offset - 5, 42, 42);
    ItemSlot item = game_state.get_players().at(playerName).get_equipped_item();
    weapon.render(item, dest, angle);
}

void SdlPlayer::load_skins() {
    ct_skins[CharacterType::Seal_Force] =
            std::make_unique<SdlTexture>("../assets/gfx/player/ct1.bmp", window);
    ct_skins[CharacterType::German_GSG_9] =
            std::make_unique<SdlTexture>("../assets/gfx/player/ct2.bmp", window);
    ct_skins[CharacterType::UK_SAS] =
            std::make_unique<SdlTexture>("../assets/gfx/player/ct3.bmp", window);
    ct_skins[CharacterType::French_GIGN] =
            std::make_unique<SdlTexture>("../assets/gfx/player/ct4.bmp", window);

    tt_skins[CharacterType::Pheonix] =
            std::make_unique<SdlTexture>("../assets/gfx/player/t1.bmp", window);
    tt_skins[CharacterType::L337_Krew] =
            std::make_unique<SdlTexture>("../assets/gfx/player/t2.bmp", window);
    tt_skins[CharacterType::Arctic_Avenger] =
            std::make_unique<SdlTexture>("../assets/gfx/player/t3.bmp", window);
    tt_skins[CharacterType::Guerrilla] =
            std::make_unique<SdlTexture>("../assets/gfx/player/t4.bmp", window);
}

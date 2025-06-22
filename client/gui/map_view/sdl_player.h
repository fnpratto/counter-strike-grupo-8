#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include "../map_view/sdl_gun.h"
#include "../window_elements/area.h"
#include "../window_elements/sdl_animation.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "client/gui/window_elements/sdl_animation.h"
#include "common/updates/game_update.h"

#include "sdl_camera.h"

class SdlPlayer {
private:
    const SdlCamera& camera;
    SdlAnimation walk_animation;

    SdlItem item;

    static constexpr int WIDTH = 32;
    static constexpr int HEIGHT = 32;
    static constexpr int x_offset = -(WIDTH / 2);
    static constexpr int y_offset = -(HEIGHT / 2);
    static constexpr const char* WALKING_ANIMATION = "../assets/gfx/player/legs_animation.xcf";
    std::unordered_map<CharacterType, std::unique_ptr<SdlTexture>> character_textures;

public:
    explicit SdlPlayer(const SdlWindow& w, const SdlCamera& camera);

    void render(const PlayerUpdate& player_update);

private:
    void load_skins(const SdlWindow& window);

    float get_rotation(const PlayerUpdate& player_state);
    Vector2D get_pos(const PlayerUpdate& player_state);
};

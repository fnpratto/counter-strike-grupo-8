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
const double PI = 3.14159265358979323846;

class SdlPlayer {
private:
    SdlWindow& window;
    const SdlCamera& camera;
    SdlAnimation walk_animation;
    const GameUpdate& game_state;
    std::string playerName;
    SDLGun weapon;

    static constexpr int WIDTH = 32;
    static constexpr int HEIGHT = 32;
    static constexpr const char* WALKING_ANIMATION = "../assets/gfx/player/legs_animation.xcf";
    std::unordered_map<CharacterType, std::unique_ptr<SdlTexture>> ct_skins;  // Use unique_ptr
    std::unordered_map<CharacterType, std::unique_ptr<SdlTexture>> tt_skins;  // Use unique_ptr
    void render_skin(const PlayerUpdate& state, int x, int y, float angle);
    void load_skins();

public:
    explicit SdlPlayer(SdlWindow& w, const SdlCamera& camera, const GameUpdate& game_state,
                       const std::string& playerName);

    void render(const PlayerUpdate& state);
};

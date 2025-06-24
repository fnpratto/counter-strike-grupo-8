#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include "../window_elements/area.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/responses.h"
#include "common/updates/game_update.h"

#include "field_of_view.h"
#include "sdl_bullet.h"
#include "sdl_camera.h"
#include "sdl_knife_slash.h"
#include "sdl_map.h"
#include "sdl_player.h"
#include "sdl_world_item.h"

class SdlWorld {
private:
    static constexpr const char* BACKGROUND_PATH = "../assets/gfx/listTeams/rectanguloxcf.xcf";
    const SdlWindow& window;
    const GameUpdate& game_state;
    const std::string& player_name;
    SdlCamera camera;
    SdlMap map;
    FieldOfView field_of_view;
    SdlTexture background;
    // TODO all players are sharing the same walk animation
    SdlPlayer player;
    std::vector<std::unique_ptr<SdlBullet> > bullets;
    std::vector<std::unique_ptr<SdlKnifeSlash> > knife_slashes;
    SdlWorldItem items;

public:
    SdlWorld(const SdlWindow& window, Map&& map, const GameUpdate& game_state,
             const std::string& player_name);

    void render();
    void renderBackground();
    void handle_hit(HitResponse&& hit);
    float get_rotation(const PlayerUpdate& player_state);
};

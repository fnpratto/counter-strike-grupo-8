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
#include "client/sound_manager.h"
#include "common/map/map.h"
#include "common/message.h"
#include "common/responses.h"
#include "common/updates/game_update.h"

#include "sdl_bullet.h"
#include "sdl_camera.h"
#include "sdl_knife_slash.h"
#include "sdl_map.h"
#include "sdl_player.h"
#include "sdl_world_item.h"

class SdlWorld {
private:
    static constexpr const char* BACKGROUND_PATH = "../assets/gfx/tiles/dust.bmp";

    const SdlWindow& window;
    const GameUpdate& game_state;
    const std::string& player_name;
    SdlCamera camera;
    SdlMap map;
    // TODO all players are sharing the same walk animation
    SdlPlayer player;
    std::vector<std::unique_ptr<SdlBullet>> bullets;
    std::vector<std::unique_ptr<SdlKnifeSlash>> knife_slashes;
    SdlWorldItem items;

    // SdlTexture background;  // TODO: Load a background texture

public:
    SdlWorld(const SdlWindow& window, Map&& map, const GameUpdate& game_state,
             const std::string& player_name);

    void render();
    std::optional<Message> getStartBombMessage(SoundManager& sound_manager);
    std::optional<Message> getStopBombMessage(SoundManager& sound_manager);
    void handle_hit(HitResponse&& hit);
};

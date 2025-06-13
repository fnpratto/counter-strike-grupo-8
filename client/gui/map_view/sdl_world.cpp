#include "sdl_world.h"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>

#include "client/gui/window_elements/area.h"
#include "client/gui/window_elements/sdl_texture.h"
#include "client/gui/window_elements/sdl_window.h"
#include "common/map/map.h"
#include "common/updates/game_update.h"

#include "sdl_camera.h"
#include "sdl_map.h"
#include "sdl_player.h"


SdlWorld::SdlWorld(SdlWindow& window, const GameUpdate& game_state, const std::string& player_name):
        window(window),
        game_state(game_state),
        player_name(player_name),
        camera(window.getWidth(), window.getHeight()),
        map(window, camera, build_default_map()) {
    for (const auto& [name, player_update]: game_state.get_players()) {
        players.emplace(name, SdlPlayer(window, camera));
    }
}

Map SdlWorld::build_default_map() {
    Map actual_map = Map("default_map", 10);

    std::vector<Floor> floors;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            floors.emplace_back(Vector2D(i * 32, j * 32));
        }
    }
    actual_map.floors = std::move(floors);

    return actual_map;
}

void SdlWorld::render() {
    camera.center(game_state.get_players().at(player_name).get_pos());

    map.render();

    for (const auto& [name, player_state]: game_state.get_players()) {
        auto [it, _] = players.emplace(name, SdlPlayer(window, camera));
        if (camera.can_see(player_state)) {
            it->second.render(player_state);
        }
    }
}

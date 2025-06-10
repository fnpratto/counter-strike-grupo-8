#include "sdl_world.h"

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
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
        players.emplace(name, SdlPlayer(window));
    }
}

Map SdlWorld::build_default_map() {
    Map actual_map = Map("default_map", 10);

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            actual_map.add_tile(MapTileType::Floor, Vector2D(i * 64, j * 64));
        }
    }

    return actual_map;
}

void SdlWorld::render() {
    // TODO Render the background
    // background.render(0, 0);

    camera.center(game_state.get_players().at(player_name).get_pos());

    map.render();

    for (auto& [name, player_state]: game_state.get_players()) {
        if (camera.can_see(player_state)) {
            auto& player = players.at(name);

            player.render(camera, player_state);
        }
    }
}

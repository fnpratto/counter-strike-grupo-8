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
#include "common/responses.h"
#include "common/updates/game_update.h"

#include "sdl_camera.h"
#include "sdl_map.h"
#include "sdl_player.h"


SdlWorld::SdlWorld(SdlWindow& window, Map&& map, const GameUpdate& game_state,
                   const std::string& player_name):
        window(window),
        game_state(game_state),
        player_name(player_name),
        camera(window.getWidth(), window.getHeight()),
        map(window, camera, std::move(map)),
        player(SdlPlayer(window, camera)) {}

void SdlWorld::handle_hit([[maybe_unused]] HitResponse&& hit) {}

void SdlWorld::render() {
    camera.center(game_state.get_players().at(player_name).get_pos());

    map.render();
    for (const auto& [_, player_state]:  // cppcheck-suppress[unusedVariable]
         game_state.get_players()) {
        player.render(player_state);
    }
}

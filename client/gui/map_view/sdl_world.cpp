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
        map(window, camera, build_default_map()),
        bullet(window) {
    for (const auto& [name, player_update]: game_state.get_players()) {
        players.emplace(name, std::make_unique<SdlPlayer>(window, camera, game_state, name));
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

void SdlWorld::handleHit(Vector2D get_origin, Vector2D get_hit_pos, Vector2D get_hit_dir,
                         bool is_hit) {

    std::cout << "Handling bullet hit: "
              << " origin: (" << get_origin.get_x() << ", " << get_origin.get_y()
              << "), hit position: (" << get_hit_pos.get_x() << ", " << get_hit_pos.get_y()
              << "), hit direction: (" << get_hit_dir.get_x() << ", " << get_hit_dir.get_y()
              << "), is hit: " << is_hit << std::endl;
    Vector2D origin_screen = camera.get_screen_pos(get_origin);
    Vector2D hit_screen = camera.get_screen_pos(get_hit_pos);
    Vector2D dir_screen = camera.get_screen_pos(get_hit_dir);

    if (game_state.get_players().at(player_name).get_equipped_item() == ItemSlot::Melee) {
        BulletInfo info{origin_screen, hit_screen, dir_screen, is_hit, true};
        for (int i = 0; i < 5; ++i) {
            bullets_info.push_back(info);
        }

    } else {
        BulletInfo info{origin_screen, hit_screen, dir_screen, is_hit, false};
        for (int i = 0; i < 5; ++i) {
            bullets_info.push_back(info);
        }
    }
}

void SdlWorld::render() {
    camera.center(game_state.get_players().at(player_name).get_pos());

    map.render();

    for (const auto& [name, player_state]: game_state.get_players()) {
        auto it = players.find(name);
        if (it == players.end()) {
            players.emplace(name, std::make_unique<SdlPlayer>(window, camera, game_state, name));
            it = players.find(name);
        }
        if (camera.can_see(player_state)) {
            it->second->render(player_state);
        }
    }

    for (const BulletInfo& info: bullets_info) {
        bullet.render(info.origin, info.hit_pos, info.hit_dir, info.is_hit, info.is_knife);
    }
    bullets_info.clear();
}

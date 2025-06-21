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


SdlWorld::SdlWorld(SdlWindow& window, Map&& map, const GameUpdate& game_state,
                   const std::string& player_name):
        window(window),
        game_state(game_state),
        player_name(player_name),
        camera(window.getWidth(), window.getHeight()),
        map(window, camera, std::move(map)),
        bullet(window),
        items(window, game_state, camera) {
    for (const auto& [name, player_update]: game_state.get_players()) {
        players.emplace(name, std::make_unique<SdlPlayer>(window, camera, game_state, name));
    }
}

void SdlWorld::handleHit(Vector2D get_origin, Vector2D get_hit_pos, Vector2D get_hit_dir,
                         bool is_hit) {
    Vector2D origin_screen = camera.get_screen_pos(get_origin);
    Vector2D hit_screen = camera.get_screen_pos(get_hit_pos);
    Vector2D dir_screen = camera.get_screen_pos(get_hit_dir);

    bool is_melee =
            (game_state.get_players().at(player_name).get_equipped_item() == ItemSlot::Melee);
    addBulletInfo(origin_screen, hit_screen, dir_screen, is_hit, is_melee);
}

void SdlWorld::addBulletInfo(const Vector2D& origin, const Vector2D& hit, const Vector2D& dir,
                             bool is_hit, bool is_melee) {
    BulletInfo info{origin, hit, dir, is_hit, is_melee};
    for (int i = 0; i < 5; ++i) {
        bullets_info.push_back(info);  // TODO RC
    }
}

void SdlWorld::render() {
    camera.center(game_state.get_players().at(player_name).get_pos());

    map.render();
    // items.render();
    for (const auto& [name, player_state]: game_state.get_players()) {
        auto it = players.find(name);
        if (it == players.end()) {
            std::cout << "Adding new player: " << name << std::endl;
            players.emplace(name, std::make_unique<SdlPlayer>(window, camera, game_state, name));
            it = players.find(name);
        }
        if (camera.can_see(player_state.get_pos())) {
            it->second->render();
        }
    }

    for (const BulletInfo& info: bullets_info) {
        bullet.render(info.origin, info.hit_pos, info.hit_dir, info.is_hit, info.is_knife);
    }
    bullets_info.clear();
}

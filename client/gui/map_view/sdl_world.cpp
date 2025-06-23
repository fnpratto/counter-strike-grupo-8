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

#include "sdl_bullet.h"
#include "sdl_camera.h"
#include "sdl_knife_slash.h"
#include "sdl_map.h"
#include "sdl_player.h"

SdlWorld::SdlWorld(const SdlWindow& window, Map&& map, const GameUpdate& game_state,
                   const std::string& player_name):
        window(window),
        game_state(game_state),
        player_name(player_name),
        camera(window.getWidth(), window.getHeight()),
        map(window, camera, std::move(map)),
        field_of_view(window, 1000.0f),
        background(BACKGROUND_PATH, window),
        player(SdlPlayer(window, camera)),
        items(window, game_state, camera) {}


void SdlWorld::handle_hit(HitResponse&& hit) {
    auto origin = game_state.get_players().at(hit.get_player_name()).get_pos();

    if (hit.get_item_slot() == ItemSlot::Melee) {
        // Handle knife slash
        knife_slashes.emplace_back(
                std::make_unique<SdlKnifeSlash>(window, camera, origin, std::move(hit)));
    } else {
        // Handle bullet
        bullets.emplace_back(std::make_unique<SdlBullet>(window, camera, origin, std::move(hit)));
    }
}

void SdlWorld::renderBackground() {

    const Area& src = Area(0, 0, 500, 300);
    const Area& dest = Area(0, 0, window.getWidth() + 100, window.getHeight());
    background.render(src, dest);
}

float SdlWorld::get_rotation(const PlayerUpdate& player_state) {
    auto aim_direction = player_state.get_aim_direction();
    float angle;
    if (aim_direction != Vector2D(0, 0)) {
        angle = std::atan2(aim_direction.get_y(), aim_direction.get_x()) * 180 / M_PI;
        angle += 90.0f;
    } else {
        angle = 0.0f;
    }
    return angle;
}

void SdlWorld::render() {
    renderBackground();
    camera.center(game_state.get_players().at(player_name).get_pos());

    map.render();
    items.render();

    for (const auto& [_, player_state]: game_state.get_players()) player.render(player_state);
    for (auto& bullet: bullets) bullet->render();
    for (auto& knife_slash: knife_slashes) knife_slash->render();

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](const auto& bullet) { return bullet->is_finished(); }),
                  bullets.end());

    knife_slashes.erase(
            std::remove_if(knife_slashes.begin(), knife_slashes.end(),
                           [](const auto& knife_slash) { return knife_slash->is_finished(); }),
            knife_slashes.end());


    field_of_view.render(window.getWidth(), window.getHeight(),
                         get_rotation(game_state.get_players().at(player_name)));
}

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

void SdlWorld::render() {
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
}


std::optional<Message> SdlWorld::getStartBombMessage(SoundManager& sound_manager) {
    std::cout << "getStartBombMessage called" << std::endl;
    Team player_team = game_state.get_players().at(player_name).get_team();
    if (player_team == Team::CT && game_state.get_phase().get_type() == PhaseType::InRound &&
        game_state.get_bomb().value().item.get_bomb_phase() == BombPhaseType::Planted) {
        sound_manager.play("defuse_bomb");
        return Message(StartDefusingBombCommand());
    } else if (player_team == Team::TT && game_state.get_phase().get_type() == PhaseType::InRound) {
        sound_manager.play("plant_bomb");
        return Message(StartPlantingBombCommand());
    }
    return std::nullopt;
}

std::optional<Message> SdlWorld::getStopBombMessage(SoundManager& sound_manager) {
    Team player_team = game_state.get_players().at(player_name).get_team();
    if (player_team == Team::CT && game_state.get_phase().get_type() == PhaseType::InRound) {
        sound_manager.play("stop_defuse_bomb");
        return Message(StopDefusingBombCommand());
    } else if (player_team == Team::TT && game_state.get_phase().get_type() == PhaseType::InRound) {
        sound_manager.play("stop_plant_bomb");
        return Message(StopPlantingBombCommand());
    }
    return std::nullopt;
}

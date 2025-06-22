#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>

#include "../window_elements/area.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "client/gui/window_elements/sdl_animation.h"
#include "common/updates/game_update.h"

#include "sdl_camera.h"
#include "tile_sheet.h"


class SdlTile {
    const SdlWindow& window;
    const SdlCamera& camera;

    static constexpr int WIDTH = 32;
    static constexpr int HEIGHT = 32;

    std::vector<SdlTexture> sheets;
    std::map<int, std::pair<std::reference_wrapper<SdlTexture>, Vector2D>> tiles;

public:
    explicit SdlTile(const SdlWindow& window, const SdlCamera& camera):
            window(window), camera(camera) {}

    void add_sheet(const TileSheet& sheet) {
        sheets.emplace_back(sheet.sheet_path, window, WIDTH, HEIGHT);
        for (const auto& [id, position]: sheet.tiles) {
            this->tiles.emplace(id, std::make_pair(std::ref(sheets.back()), position));
        }
    }

    void render(const Tile& tile) {
        if (!camera.can_see(tile))
            return;

        auto position_from_cam = camera.get_screen_pos(tile.pos);

        auto [sheet, position] = tiles.at(tile.id);
        SDL_Rect src_rect = {position.get_x(), position.get_y(), WIDTH, HEIGHT};

        Area src(src_rect.x, src_rect.y, src_rect.w, src_rect.h);
        Area dest(position_from_cam.get_x(), position_from_cam.get_y(), src_rect.w, src_rect.h);

        sheet.get().render(src, dest);
    }
};

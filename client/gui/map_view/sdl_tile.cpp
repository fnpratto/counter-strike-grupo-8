#include "sdl_tile.h"

SdlTile::SdlTile(const SdlWindow& window, const SdlCamera& camera):
        window(window), camera(camera) {}

void SdlTile::add_sheet(const TileSheet& sheet) {
    auto sheet_texture = std::make_shared<SdlTexture>(sheet.sheet_path, window, WIDTH, HEIGHT);
    for (const auto& [id, position]: sheet.tiles) {
        this->tiles.emplace(id, std::make_pair(sheet_texture, position));
    }
}

void SdlTile::render(const Tile& tile) {
    if (!camera.can_see(tile))
        return;

    auto position_from_cam = camera.get_screen_pos(tile.pos);

    auto [sheet, position] = tiles.at(tile.id);
    SDL_Rect src_rect = {position.get_x(), position.get_y(), WIDTH, HEIGHT};

    Area src(src_rect.x, src_rect.y, src_rect.w, src_rect.h);
    Area dest(position_from_cam.get_x(), position_from_cam.get_y(), src_rect.w, src_rect.h);

    sheet.get()->render(src, dest);
}

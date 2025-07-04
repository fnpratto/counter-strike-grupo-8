#include "sdl_map.h"

#include <stdexcept>
#include <utility>

#include "sdl_camera.h"
#include "tile_loader.h"

SdlMap::SdlMap(const SdlWindow& w, const SdlCamera& camera, Map&& map):
        window(w), map(std::move(map)), tile(w, camera) {
    TileLoader loader(this->map);
    loader.load_into(this->tile);
}

void SdlMap::render() {
    for (int x = 0; x < map.get_width(); ++x) {
        for (int y = 0; y < map.get_height(); ++y) {
            const auto& tile_opt = map.get_tiles().at(x).at(y);
            if (!tile_opt.has_value())
                continue;

            const Tile& tile_map = tile_opt.value();
            this->tile.render(tile_map);
        }
    }
}

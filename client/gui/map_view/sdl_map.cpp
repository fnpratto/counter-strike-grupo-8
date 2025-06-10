#include "sdl_map.h"

#include <stdexcept>
#include <utility>

SdlMap::SdlMap(SdlWindow& window, const SdlCamera& camera, Map&& map):
        window(window), camera(camera), map(std::move(map)), tile(window, camera) {}

void SdlMap::render() {
    for (const auto& map_tile: map.get_tiles()) {
        if (camera.can_see(map_tile)) {
            tile.render(map_tile);
        }
    }
}

#include "sdl_map.h"

#include <stdexcept>
#include <utility>

#include "sdl_camera.h"

SdlMap::SdlMap(SdlWindow& window, const SdlCamera& camera, Map&& map):
        window(window), camera(camera), map(std::move(map)), tile(window, camera) {}

void SdlMap::render() {
    for (const auto& map_wall: map.get_walls())
        if (camera.can_see(map_wall))
            tile.render(map_wall);

    for (const auto& map_floor: map.get_floors())
        if (camera.can_see(map_floor))
            tile.render(map_floor);

    for (const auto& map_box: map.get_boxes())
        if (camera.can_see(map_box))
            tile.render(map_box);
}

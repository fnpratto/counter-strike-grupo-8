#include "sdl_map.h"

#include <stdexcept>
#include <utility>

#include "sdl_camera.h"

SdlMap::SdlMap(SdlWindow& w, const SdlCamera& camera, Map&& map):
        window(w), camera(camera), map(std::move(map)), tile(w, camera) {}

void SdlMap::render() {
    for (const auto& map_wall: map.walls)
        if (camera.can_see(map_wall))
            tile.render(map_wall);

    for (const auto& map_floor: map.floors)
        if (camera.can_see(map_floor))
            tile.render(map_floor);

    for (const auto& map_box: map.boxes)
        if (camera.can_see(map_box))
            tile.render(map_box);
}

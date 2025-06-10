#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>

#include "common/map/tile.h"
#include "common/updates/player_update.h"

class SdlCamera {
private:
    static constexpr float meter_u = 64.0f;
    static constexpr float meter_px = 32.0f;
    static constexpr float u2px_rate = meter_px / meter_u;


    int screen_width;
    int screen_height;
    Vector2D position;

public:
    SdlCamera(int screen_width, int screen_height);
    void center(const Vector2D& target_pos);

    Vector2D get_offset(Vector2D position) const;

    bool can_see(const PlayerUpdate& player) const { return can_see(player.get_pos()); }
    bool can_see(const Tile& tile) const { return can_see(tile.get_pos()); }

private:
    bool can_see(const Vector2D& target_pos) const;
};


#endif  // CAMERA_H

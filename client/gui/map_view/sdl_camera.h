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
#include "common/physics/physics_config.h"
#include "common/updates/player_update.h"

class SdlCamera {
private:
    static constexpr float meter_u = static_cast<float>(PhysicsConfig::meter_size);
    static constexpr float meter_px = 32.0f;
    static constexpr float u2px_rate = meter_px / meter_u;


    int screen_width;
    int screen_height;
    Vector2D position;
    float field_of_view;

public:
    SdlCamera(int screen_width, int screen_height);
    void center(const Vector2D& target_pos);

    Vector2D get_screen_pos(Vector2D position) const;

    bool can_see(const Vector2D& pos) const;

    bool can_see(const Tile& obj) const;

    Vector2D get_position() const { return position; }
};
#endif  // CAMERA_H

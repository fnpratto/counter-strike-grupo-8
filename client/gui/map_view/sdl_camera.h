#pragma once

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

public:
    SdlCamera(int screen_width, int screen_height);
    void center(const Vector2D& target_pos);

    Vector2D get_screen_pos(Vector2D position) const;

    /**
     * @brief Checks if a game object is within the camera's view.
     *
     * @param obj The game object to check. (must respond to `.get_pos()`)
     * @return true if the object is within view, false otherwise.
     */
    template <typename T>
    bool can_see(const T& obj) const;

    bool can_see(const Vector2D& pos) const;
    bool can_see(const Tile& obj) const;
};

#pragma once

#include <map>
#include <memory>
#include <string>

#include "server/map/map.h"
#include "server/player/player.h"
#include "server/utils/vector_2d.h"

class PhysicsSystem {
private:
    const Map& map;
    const std::map<std::string, std::unique_ptr<Player>>& players;

public:
    PhysicsSystem(const Map& map, const std::map<std::string, std::unique_ptr<Player>>& players);

    Vector2D random_spawn_tt_pos() const;
    Vector2D random_spawn_ct_pos() const;

    Vector2D calculate_step(const Vector2D& dir) const;
};

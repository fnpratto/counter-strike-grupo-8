#pragma once

#include "common/utils/vector_2d.h"

// Forward declaration
class Player;

class AttackEffect {
protected:
    const Player& player_origin;
    int damage;
    Vector2D dir;

    virtual int compute_damage(int distance) const = 0;

    virtual bool is_target_hit() const = 0;

public:
    AttackEffect(const Player& player_origin, int damage, const Vector2D& dir):
            player_origin(player_origin), damage(damage), dir(dir) {}

    // TODO: void apply(const Player& player) {}
    // When applied:
    //      - Target take damage
    //      - If target dies, then target (internally) updates its scoreboard
    //        incrementing number of deads. Origin's scoreboard should be updated
    //        too, incrementing its number of kills.

    virtual ~AttackEffect() = default;
};

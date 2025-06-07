#pragma once

#include "common/utils/vector_2d.h"

class AttackEffect {
protected:
    // TODO: const Player& origin;
    int damage;

    virtual int compute_damage(int distance) const = 0;

    virtual bool is_target_hit() const = 0;

public:
    explicit AttackEffect(int damage): damage(damage) {}

    // TODO: void apply(const Player& player) {}
    // When applied:
    //      - Target take damage
    //      - If target dies, then target (internally) updates its scoreboard
    //        incrementing number of deads. Origin's scoreboard should be updated
    //        too, incrementing its number of kills.

    virtual ~AttackEffect() = default;
};

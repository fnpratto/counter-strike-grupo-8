#pragma once

#include "attack_effect.h"

class GunAttack: public AttackEffect {
protected:
    float precision;
    float falloff;

public:
    GunAttack(Player& player_origin, int damage, const Vector2D& dir, int max_range,
              float precision, float falloff);

    int compute_damage(int distance) const override;

    bool is_target_hit() const override;
};

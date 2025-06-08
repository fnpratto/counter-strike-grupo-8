#pragma once

#include "attack_effect.h"

class GunAttack: public AttackEffect {
protected:
    float precision;

public:
    GunAttack(const Player& player_origin, int damage, const Vector2D& dir, float precision):
            AttackEffect(player_origin, damage, dir), precision(precision) {}

    int compute_damage(int distance) const override {
        // TODO: Implement falloff for GunAttack
        (void)distance;
        return damage;
    }

    bool is_target_hit() const override {
        return RandomFloatGenerator(0, 100).generate() <= precision;
    }
};

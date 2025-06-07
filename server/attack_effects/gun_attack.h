#pragma once

#include "attack_effect.h"

class GunAttack: public AttackEffect {
protected:
    const Vector2D& dir;
    float precision;

public:
    // TODO: Instead of passing dir, we shoul take it from player reference
    GunAttack(int damage, const Vector2D& dir, float precision):
            AttackEffect(damage), dir(dir), precision(precision) {}

    Vector2D get_dir() const { return dir; }

    int compute_damage(int distance) const override {
        // TODO: Implement falloff for GunAttack
        (void)distance;
        return damage;
    }

    bool is_target_hit() const override {
        return RandomFloatGenerator(0, 100).generate() <= precision;
    }
};

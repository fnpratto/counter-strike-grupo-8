#pragma once

#include <memory>

#include "common/utils/vector_2d.h"

class Player;

class Effect {
protected:
    Vector2D origin;
    int damage;
    int max_range;
    float precision;
    float falloff;

    bool is_target_hit(int distance) const;

    int compute_damage(int distance) const;

    float apply_falloff(float value, int distance) const;

public:
    Effect(const Vector2D& origin, int damage, int max_range, float precision, float falloff);

    Effect(const Effect&) = delete;
    Effect& operator=(const Effect&) = delete;

    Effect(Effect&&) = default;
    Effect& operator=(Effect&&) = default;

    Vector2D get_origin() const;
    int get_max_range() const;

    bool apply(const std::unique_ptr<Player>& player) const;
};

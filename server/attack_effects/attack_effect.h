#pragma once

#include <memory>

#include "common/utils/vector_2d.h"

class Player;

class AttackEffect {
protected:
    Player& player_origin;
    int damage;
    Vector2D dir;
    int max_range;

    virtual int compute_damage(int distance) const = 0;

    virtual bool is_target_hit() const = 0;

public:
    AttackEffect(Player& player_origin, int damage, const Vector2D& dir, int max_range);

    Vector2D get_dir() const;
    int get_max_range() const;

    bool apply(const std::unique_ptr<Player>& player, int distance);

    virtual ~AttackEffect() = default;
};

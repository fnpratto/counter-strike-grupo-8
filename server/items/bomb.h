#pragma once

#include "effects/effect.h"
#include "server/clock/clock.h"
#include "server/logic.h"
#include "server/states/bomb_state.h"

class Bomb: public Logic<BombState, BombUpdate> {
    TimePoint action_start_time = TimePoint::max();

public:
    Bomb();

    Bomb(const Bomb&) = delete;
    Bomb& operator=(const Bomb&) = delete;

    Bomb(Bomb&&) = default;
    Bomb& operator=(Bomb&&) = default;

    bool is_planted() const;
    bool should_explode() const;
    bool get_is_exploded() const;
    bool get_is_defused() const;

    void start_planting(TimePoint now);
    void stop_planting(TimePoint now);

    void start_defusing(TimePoint now);

    void advance(TimePoint now);

    Effect explode(const Vector2D& origin);

    void reset();
};

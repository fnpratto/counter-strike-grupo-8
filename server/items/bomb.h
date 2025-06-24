#pragma once

#include "effects/effect.h"
#include "server/clock/clock.h"
#include "server/logic.h"
#include "server/states/bomb_state.h"

class Bomb: public Logic<BombState, BombUpdate> {
    TimePoint phase_start_time;
    TimePoint plant_time;

    void change_bomb_phase(BombPhaseType new_phase, TimePoint now);

public:
    Bomb();

    Bomb(const Bomb&) = delete;
    Bomb& operator=(const Bomb&) = delete;

    Bomb(Bomb&&) = default;
    Bomb& operator=(Bomb&&) = default;

    bool is_planting() const;
    bool is_planted() const;
    bool is_defusing() const;
    bool is_defused() const;
    bool is_exploded() const;
    bool should_explode() const;

    void start_planting(TimePoint now);
    void stop_planting(TimePoint now);
    void start_defusing(TimePoint now);
    void stop_defusing(TimePoint now);

    void advance(TimePoint now);

    Effect explode(const Vector2D& origin);

    void reset();
};

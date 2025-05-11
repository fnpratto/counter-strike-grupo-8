#pragma once

#include <chrono>

#include "common/models.h"
#include "clock/clock.h"

class GamePhase {
private:
    const Clock& clock;
    GamePhaseType phase;
    std::chrono::steady_clock::time_point phase_start;

public:
    explicit GamePhase(const Clock& clock);

    GamePhaseType get_type() const;
    int get_buying_phase_duration() const;
    int get_playing_phase_duration() const;

    void start();

    void update();
};

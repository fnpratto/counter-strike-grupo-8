#pragma once

#include <chrono>

#include "common/models.h"
#include "server/clock/clock.h"

class GamePhase {
private:
    const Clock& clock;
    PhaseType phase;
    std::chrono::steady_clock::time_point phase_start;
    int buying_phase_secs;
    int playing_phase_secs;

public:
    GamePhase(const Clock& clock, int buying_phase_secs, int playing_phase_secs);

    bool is_started() const;

    PhaseType get_type() const;

    void start_buying_phase();

    void update();
};

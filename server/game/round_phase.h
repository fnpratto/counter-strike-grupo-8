#pragma once

#include <chrono>

#include "common/models.h"
#include "server/clock/clock.h"

class RoundPhase {
private:
    const Clock& clock;
    RoundPhaseType phase;
    std::chrono::steady_clock::time_point phase_start;
    int buying_phase_secs;
    int playing_phase_secs;

public:
    RoundPhase(const Clock& clock, int buying_phase_secs, int playing_phase_secs);

    RoundPhaseType get_type() const;

    void start_buying_phase();

    void update();
};

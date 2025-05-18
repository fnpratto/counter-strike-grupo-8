#pragma once

#include <chrono>

#include "common/models.h"
#include "server/clock/clock.h"

class GamePhase {
private:
    const Clock& clock;
    PhaseType phase;
    TimePoint phase_start;

public:
    GamePhase(const Clock& clock);

    bool is_started() const;
    bool is_round_finished() const;
    bool is_buying_phase() const;

    void start_buying_phase();

    void update();
};

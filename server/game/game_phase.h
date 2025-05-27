#pragma once

#include <chrono>

#include "common/game_state.h"
#include "common/game_state_update.h"
#include "common/models.h"
#include "server/clock/clock.h"

class GamePhase {
private:
    const Clock& clock;
    PhaseType phase;
    TimePoint phase_start;
    PhaseUpdate updates;

public:
    explicit GamePhase(const Clock& clock);

    bool is_started() const;
    bool is_round_finished() const;
    bool is_buying_phase() const;

    PhaseUpdate get_updates() const;

    void clear_updates();

    PhaseState full_state() const;

    void start_buying_phase();

    void advance();
};

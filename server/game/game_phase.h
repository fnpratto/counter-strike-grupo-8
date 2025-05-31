#pragma once

#include <memory>

#include "common/phase_state.h"
#include "common/updates/phase_update.h"
#include "server/clock/clock.h"
#include "server/logic.h"

class GamePhase: public Logic<PhaseState, PhaseUpdate> {
private:
    std::unique_ptr<Clock> game_clock;
    TimePoint phase_start;

public:
    explicit GamePhase(std::unique_ptr<Clock>&& game_clock);

    bool is_started() const;
    bool is_round_finished() const;
    bool is_buying_phase() const;

    void start_buying_phase();

    void advance();
};

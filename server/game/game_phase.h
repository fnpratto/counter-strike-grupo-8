#pragma once

#include <memory>

#include "common/updates/phase_update.h"
#include "server/clock/clock.h"
#include "server/logic.h"
#include "server/states/phase_state.h"

class GamePhase: public Logic<PhaseState, PhaseUpdate> {
private:
    std::shared_ptr<Clock> game_clock;
    TimePoint phase_start;
    bool round_finished;

public:
    explicit GamePhase(std::shared_ptr<Clock>&& game_clock);

    bool is_started() const;
    bool round_has_finished() const;
    bool is_buying_phase() const;
    bool is_playing_phase() const;

    TimePoint get_time_now() const;

    void start_buying_phase();

    void advance();
};

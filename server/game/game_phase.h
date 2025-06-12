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

    std::chrono::seconds get_current_phase_secs() const;
    PhaseType get_next_phase() const;

public:
    explicit GamePhase(std::shared_ptr<Clock>&& game_clock);

    GamePhase(const GamePhase&) = delete;
    GamePhase& operator=(const GamePhase&) = delete;

    GamePhase(GamePhase&&) = default;
    GamePhase& operator=(GamePhase&&) = default;

    PhaseType get_type() const;

    bool is_started() const;
    bool is_buying_phase() const;
    bool is_playing_phase() const;
    bool is_round_finished() const;

    TimePoint get_time_now() const;

    void start_buying_phase();

    /**
     * Return true if the game phase type changed
     */
    bool advance();
};

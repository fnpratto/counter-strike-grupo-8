#pragma once

#include <map>
#include <memory>

#include "common/updates/phase_update.h"
#include "server/clock/clock.h"
#include "server/logic.h"
#include "server/states/phase_state.h"

#include "game_config.h"

class GamePhase: public Logic<PhaseState, PhaseUpdate> {
private:
    std::shared_ptr<Clock> game_clock;
    TimePoint phase_start;
    std::map<PhaseType, unsigned int> phase_durations;  // seconds
    std::map<PhaseType, PhaseType> next_phase_map;

public:
    explicit GamePhase(std::shared_ptr<Clock>&& game_clock,
                       const GameConfig::PhaseTimes& phase_times);

    GamePhase(const GamePhase&) = delete;
    GamePhase& operator=(const GamePhase&) = delete;

    GamePhase(GamePhase&&) = default;
    GamePhase& operator=(GamePhase&&) = default;

    PhaseType get_type() const;

    bool is_playing() const;
    bool is_buying_phase() const;
    bool is_round_end() const;
    bool is_game_end() const;

    TimePoint get_time_now() const;

    void start_game();
    void end_game();
    void start_bomb_planted_phase();
    void end_round();

    /**
     * Return true if the game phase type changed
     */
    bool advance();
};

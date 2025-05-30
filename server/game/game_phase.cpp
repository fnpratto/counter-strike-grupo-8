#include "game_phase.h"

#include <memory>
#include <utility>

#include "game_config.h"

GamePhase::GamePhase(std::unique_ptr<Clock>&& gc):
        game_clock(std::move(gc)), phase(PhaseType::WarmUp) {}

bool GamePhase::is_started() const { return phase != PhaseType::WarmUp; }

bool GamePhase::is_round_finished() const { return phase == PhaseType::RoundFinished; }

bool GamePhase::is_buying_phase() const { return phase == PhaseType::Buying; }

PhaseUpdate GamePhase::get_updates() const { return updates; }

void GamePhase::clear_updates() { updates.clear(); }

PhaseState GamePhase::full_state() const {
    PhaseState phase_state;
    phase_state.phase = phase;
    phase_state.time = game_clock->now();
    return phase_state;
}

void GamePhase::start_buying_phase() {
    phase = PhaseType::Buying;
    updates.set_phase(phase);
    phase_start = game_clock->now();
}

void GamePhase::advance() {
    auto now = game_clock->now();
    updates.set_time(now);
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - phase_start);

    std::chrono::seconds phase_duration;
    switch (phase) {
        case PhaseType::Buying:
            phase_duration = std::chrono::seconds(PhaseTimes::buying_phase_secs);
            if (elapsed >= phase_duration) {
                phase = PhaseType::Playing;
                updates.set_phase(phase);
                phase_start = now;
            }
            break;
        case PhaseType::Playing:
            phase_duration = std::chrono::seconds(PhaseTimes::playing_phase_secs);
            if (elapsed >= phase_duration) {
                phase = PhaseType::RoundFinished;
                updates.set_phase(phase);
                phase_start = now;
            }
            break;
        case PhaseType::RoundFinished:
            phase_duration = std::chrono::seconds(PhaseTimes::round_finished_phase_secs);
            if (elapsed >= phase_duration) {
                phase = PhaseType::Buying;
                updates.set_phase(phase);
                phase_start = now;
            }
            break;
        default:
            break;
    }
}

#include "game_phase.h"

#include <memory>
#include <utility>

#include "game_config.h"

GamePhase::GamePhase(std::unique_ptr<Clock>&& gc):
        game_clock(std::move(gc)), state(PhaseType::WarmUp, game_clock->now()) {}

bool GamePhase::is_started() const { return state.get_phase() != PhaseType::WarmUp; }

bool GamePhase::is_round_finished() const { return state.get_phase() == PhaseType::RoundFinished; }

bool GamePhase::is_buying_phase() const { return state.get_phase() == PhaseType::Buying; }

void GamePhase::clear_updates() { state.clear_updates(); }

PhaseState GamePhase::get_state() const { return state; }

void GamePhase::start_buying_phase() {
    state.set_phase(PhaseType::Buying);
    phase_start = game_clock->now();
}

void GamePhase::advance() {
    auto now = game_clock->now();
    state.set_time(now);
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - phase_start);

    std::chrono::seconds phase_duration;
    switch (state.get_phase()) {
        case PhaseType::Buying:
            phase_duration = std::chrono::seconds(PhaseTimes::buying_phase_secs);
            if (elapsed >= phase_duration) {
                state.set_phase(PhaseType::Playing);
                phase_start = now;
            }
            break;
        case PhaseType::Playing:
            phase_duration = std::chrono::seconds(PhaseTimes::playing_phase_secs);
            if (elapsed >= phase_duration) {
                state.set_phase(PhaseType::RoundFinished);
                phase_start = now;
            }
            break;
        case PhaseType::RoundFinished:
            phase_duration = std::chrono::seconds(PhaseTimes::round_finished_phase_secs);
            if (elapsed >= phase_duration) {
                state.set_phase(PhaseType::Buying);
                phase_start = now;
            }
            break;
        default:
            break;
    }
}

#include "game_phase.h"

#include <memory>
#include <utility>

#include "game_config.h"

GamePhase::GamePhase(std::shared_ptr<Clock>&& game_clock):
        Logic<PhaseState, PhaseUpdate>(PhaseState(PhaseType::WarmUp)),
        game_clock(std::move(game_clock)) {
    phase_start = this->game_clock->now();
}

PhaseType GamePhase::get_type() const { return state.get_phase(); }

bool GamePhase::is_started() const { return state.get_phase() != PhaseType::WarmUp; }

bool GamePhase::is_buying_phase() const { return state.get_phase() == PhaseType::Buying; }

bool GamePhase::is_playing_phase() const { return state.get_phase() == PhaseType::Playing; }

bool GamePhase::is_round_finished() const { return state.get_phase() == PhaseType::End; }

TimePoint GamePhase::get_time_now() const { return game_clock->now(); }

void GamePhase::start_buying_phase() {
    state.set_phase(PhaseType::Buying);
    phase_start = game_clock->now();
}

bool GamePhase::advance() {
    PhaseType old_phase = state.get_phase();
    auto now = game_clock->now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - phase_start);
    if (state.get_phase() != PhaseType::WarmUp && elapsed >= get_current_phase_secs()) {
        state.set_phase(get_next_phase());
        phase_start = now;
    }

    if (state.get_phase() == PhaseType::WarmUp)
        state.set_secs_remaining(0);
    else
        state.set_secs_remaining(
                std::chrono::duration<int>(get_current_phase_secs() - elapsed).count());

    return old_phase != state.get_phase();
}

std::chrono::seconds GamePhase::get_current_phase_secs() const {
    switch (state.get_phase()) {
        case PhaseType::Buying:
            return std::chrono::seconds(PhaseTimes::buying_phase_secs);
        case PhaseType::Playing:
            return std::chrono::seconds(PhaseTimes::playing_phase_secs);
        case PhaseType::End:
            return std::chrono::seconds(PhaseTimes::end_phase_secs);
        default:
            throw std::runtime_error("Invalid phase type");
    }
}

PhaseType GamePhase::get_next_phase() const {
    switch (state.get_phase()) {
        case PhaseType::Buying:
            return PhaseType::Playing;
        case PhaseType::Playing:
            return PhaseType::End;
        case PhaseType::End:
            return PhaseType::Buying;
        default:
            throw std::runtime_error("Invalid phase type");
    }
}

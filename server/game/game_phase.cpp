#include "game_phase.h"

#include <memory>
#include <utility>

#include "game_config.h"

GamePhase::GamePhase(std::shared_ptr<Clock>&& game_clock):
        Logic<PhaseState, PhaseUpdate>(PhaseState(PhaseType::WarmUp, TimePoint::min())),
        game_clock(std::move(game_clock)),
        round_finished(false) {
    phase_start = this->game_clock->now();
}

bool GamePhase::is_started() const { return state.get_phase() != PhaseType::WarmUp; }

bool GamePhase::round_has_finished() const { return round_finished; }

bool GamePhase::is_buying_phase() const { return state.get_phase() == PhaseType::Buying; }

void GamePhase::start_buying_phase() {
    state.set_phase(PhaseType::Buying);
    phase_start = game_clock->now();
}

void GamePhase::advance() {
    auto now = game_clock->now();
    state.set_time(now);
    round_finished = false;
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - phase_start);

    switch (state.get_phase()) {
        case PhaseType::Buying:
            if (elapsed >= std::chrono::seconds(PhaseTimes::buying_phase_secs)) {
                state.set_phase(PhaseType::Playing);
                phase_start = now;
            }
            break;
        case PhaseType::Playing:
            if (elapsed >= std::chrono::seconds(PhaseTimes::playing_phase_secs)) {
                state.set_phase(PhaseType::End);
                phase_start = now;
            }
            break;
        case PhaseType::End:
            if (elapsed >= std::chrono::seconds(PhaseTimes::round_finished_phase_secs)) {
                state.set_phase(PhaseType::Buying);
                round_finished = true;
                phase_start = now;
            }
            break;
        default:
            break;
    }
}

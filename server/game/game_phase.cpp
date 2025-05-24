#include "game_phase.h"

#include "server/cons.h"

GamePhase::GamePhase(const Clock& clock): clock(clock), phase(PhaseType::WarmUp) {}

bool GamePhase::is_started() const { return phase != PhaseType::WarmUp; }

bool GamePhase::is_round_finished() const { return phase == PhaseType::RoundFinished; }

bool GamePhase::is_buying_phase() const { return phase == PhaseType::Buying; }

void GamePhase::start_buying_phase() {
    phase = PhaseType::Buying;
    phase_start = clock.now();
}

void GamePhase::update() {
    auto now = clock.now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - phase_start);

    std::chrono::seconds phase_duration;
    switch (phase) {
        case PhaseType::Buying:
            phase_duration = std::chrono::seconds(PhaseTimes::buying_phase_secs);
            if (elapsed >= phase_duration) {
                phase = PhaseType::Playing;
                phase_start = now;
            }
            break;
        case PhaseType::Playing:
            phase_duration = std::chrono::seconds(PhaseTimes::playing_phase_secs);
            if (elapsed >= phase_duration) {
                phase = PhaseType::RoundFinished;
                phase_start = now;
            }
            break;
        case PhaseType::RoundFinished:
            phase_duration = std::chrono::seconds(PhaseTimes::round_finished_phase_secs);
            if (elapsed >= phase_duration) {
                phase = PhaseType::Buying;
                phase_start = now;
            }
            break;
        default:
            break;
    }
}

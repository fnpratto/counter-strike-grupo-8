#include "game_phase.h"

GamePhase::GamePhase(const Clock& clock, int buying_phase_secs, int playing_phase_secs) : 
        clock(clock),
        phase(PhaseType::NotStarted),
        buying_phase_secs(buying_phase_secs),
        playing_phase_secs(playing_phase_secs) {}

bool GamePhase::is_started() const { return phase != PhaseType::NotStarted; }

PhaseType GamePhase::get_type() const { return phase; }

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
            phase_duration = std::chrono::seconds(buying_phase_secs);
            if (elapsed >= phase_duration) {
                phase = PhaseType::Playing;
                phase_start = now;
            }
            break;
        case PhaseType::Playing:
            phase_duration = std::chrono::seconds(playing_phase_secs);
            if (elapsed >= phase_duration) {
                phase = PhaseType::RoundFinished;
                phase_start = now;
            }
            break;
        case PhaseType::RoundFinished:
            // TODO: PhaseType::RoundFinished could also have a duration
            phase = PhaseType::Buying;
            break;
        default:
            break;
    }
}

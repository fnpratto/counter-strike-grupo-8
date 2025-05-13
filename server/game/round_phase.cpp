#include "round_phase.h"

RoundPhase::RoundPhase(const Clock& clock, int buying_phase_secs, int playing_phase_secs) : 
        clock(clock),
        phase(RoundPhaseType::NotStarted),
        buying_phase_secs(buying_phase_secs),
        playing_phase_secs(playing_phase_secs) {}

RoundPhaseType RoundPhase::get_type() const { return phase; }

void RoundPhase::start_buying_phase() {
    phase = RoundPhaseType::Buying;
    phase_start = clock.now();
}

void RoundPhase::update() {
    auto now = clock.now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - phase_start);

    std::chrono::seconds phase_duration;
    switch (phase) {
        case RoundPhaseType::Buying:
            phase_duration = std::chrono::seconds(buying_phase_secs);
            if (elapsed >= phase_duration) {
                phase = RoundPhaseType::Playing;
                phase_start = now;
            }
            break;
        case RoundPhaseType::Playing:
            phase_duration = std::chrono::seconds(playing_phase_secs);
            if (elapsed >= phase_duration) {
                phase = RoundPhaseType::Finished;
                phase_start = now;
            }
            break;
        case RoundPhaseType::Finished:
            // TODO: RoundPhaseType::Finished could also have a duration
            phase = RoundPhaseType::Buying;
            break;
        default:
            break;
    }
}

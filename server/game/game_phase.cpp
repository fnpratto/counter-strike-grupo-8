#include "game_phase.h"

#define BUYING_DUR_SECS 30
#define PLAYING_DUR_SECS 120

GamePhase::GamePhase(const Clock& clock) : 
        clock(clock),
        phase(GamePhaseType::NotStarted) {}

GamePhaseType GamePhase::get_type() const { return phase; }

int GamePhase::get_buying_phase_duration() const { return BUYING_DUR_SECS; }
    
int GamePhase::get_playing_phase_duration() const { return PLAYING_DUR_SECS; }

void GamePhase::start() {
    phase = GamePhaseType::Buying;
    phase_start = clock.now();
}

void GamePhase::update() {
    auto now = clock.now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - phase_start);

    std::chrono::seconds phase_duration;
    switch (phase) {
        case GamePhaseType::Buying:
            phase_duration = std::chrono::seconds(BUYING_DUR_SECS);
            if (elapsed >= phase_duration) {
                phase = GamePhaseType::Playing;
                phase_start = now;
            }
            break;
        case GamePhaseType::Playing:
            phase_duration = std::chrono::seconds(PLAYING_DUR_SECS);
            if (elapsed >= phase_duration) {
                phase = GamePhaseType::Finished;
                phase_start = now;
            }
            break;
        default:
            break;
    }
}

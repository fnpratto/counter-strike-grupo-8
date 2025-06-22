#include "game_phase.h"

#include <memory>
#include <utility>

#include "game_config.h"

GamePhase::GamePhase(std::shared_ptr<Clock>&& game_clock):
        Logic<PhaseState, PhaseUpdate>(PhaseState(PhaseType::WarmUp)),
        game_clock(std::move(game_clock)),
        phase_durations({{PhaseType::WarmUp, 0},
                         {PhaseType::Buying, PhaseTimes::buying_duration},
                         {PhaseType::InRound, PhaseTimes::round_duration},
                         {PhaseType::RoundEnd, PhaseTimes::round_end_duration},
                         {PhaseType::GameEnd, 0}}),
        next_phase_map({{PhaseType::WarmUp, PhaseType::Buying},
                        {PhaseType::Buying, PhaseType::InRound},
                        {PhaseType::InRound, PhaseType::RoundEnd},
                        {PhaseType::BombPlanted, PhaseType::RoundEnd},
                        {PhaseType::RoundEnd, PhaseType::Buying}}) {
    phase_start = this->game_clock->now();
}

PhaseType GamePhase::get_type() const { return state.get_phase(); }

bool GamePhase::is_playing() const {
    return state.get_phase() != PhaseType::WarmUp && state.get_phase() != PhaseType::GameEnd;
}

bool GamePhase::is_buying_phase() const { return state.get_phase() == PhaseType::Buying; }

bool GamePhase::is_round_end() const { return state.get_phase() == PhaseType::RoundEnd; }

bool GamePhase::is_game_end() const { return state.get_phase() == PhaseType::GameEnd; }

TimePoint GamePhase::get_time_now() const { return game_clock->now(); }

void GamePhase::start_game() {
    state.set_phase(PhaseType::Buying);
    phase_start = game_clock->now();
    state.set_secs_remaining(phase_durations.at(state.get_phase()));
}

void GamePhase::end_game() {
    state.set_phase(PhaseType::GameEnd);
    phase_start = game_clock->now();
    state.set_secs_remaining(phase_durations.at(state.get_phase()));
}

void GamePhase::start_bomb_planted_phase() {
    if (!is_playing())
        return;
    state.set_phase(PhaseType::BombPlanted);
    phase_start = game_clock->now();
}

void GamePhase::end_round() {
    if (!is_playing())
        return;
    state.set_phase(PhaseType::RoundEnd);
    phase_start = game_clock->now();
    state.set_secs_remaining(phase_durations.at(state.get_phase()));
}

bool GamePhase::advance() {
    PhaseType old_phase = state.get_phase();
    auto now = game_clock->now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - phase_start);
    if (is_playing() && state.get_phase() != PhaseType::BombPlanted) {
        auto current_phase_duration = phase_durations.at(state.get_phase());
        state.set_secs_remaining(current_phase_duration - elapsed.count());
        if (elapsed.count() >= current_phase_duration) {
            state.set_phase(next_phase_map.at(state.get_phase()));
            phase_start = now;
        }
    }

    return old_phase != state.get_phase();
}

#pragma once

namespace GameConfig {
static constexpr int tickrate = 30;
static constexpr int player_speed = 2;  // m/s
static constexpr int max_rounds = 10;
};  // namespace GameConfig

namespace PhaseTimes {
const int buying_phase_secs = 30;
const int playing_phase_secs = 120;
const int round_finished_phase_secs = 5;
};  // namespace PhaseTimes

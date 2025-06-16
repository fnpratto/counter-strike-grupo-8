#pragma once

namespace GameConfig {
static constexpr int tickrate = 30;
static constexpr int player_speed = 2;  // m/s
static constexpr int max_rounds = 10;
};  // namespace GameConfig

namespace PhaseTimes {
static constexpr int buying_phase_secs = 30;
static constexpr int playing_phase_secs = 120;
static constexpr int round_end_phase_secs = 5;
static constexpr int game_end_phase_secs = 10;
};  // namespace PhaseTimes

namespace Scores {
static constexpr int kill = 1;
static constexpr int win = 5;
static constexpr int loss = 0;
};  // namespace Scores

namespace Bonifications {
static constexpr int kill = 300;
static constexpr int win = 3000;
static constexpr int loss = 1500;
}  // namespace Bonifications

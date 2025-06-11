#pragma once

namespace GameConfig {
static constexpr int tickrate = 30;
static constexpr int player_speed = 2;  // m/s
static constexpr int max_rounds = 10;
};  // namespace GameConfig

namespace PhaseTimes {
const int buying_phase_secs = 30;
const int playing_phase_secs = 120;
const int end_phase_secs = 5;
};  // namespace PhaseTimes

namespace ScoresConfig {
const int kill = 2;
// TODO: const int win = 5;
};  // namespace ScoresConfig

namespace BonificationsConfig {
const int kill = 100;
// TODO: const int win = 300;
}  // namespace BonificationsConfig

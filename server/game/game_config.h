#pragma once

namespace GameConfig {
static constexpr int tickrate = 30;
static constexpr int player_speed = 2;  // m/s
static constexpr int max_rounds = 10;
};  // namespace GameConfig

namespace PhaseTimes {
static constexpr int buying_phase_secs = 30;
static constexpr int playing_phase_secs = 120;
static constexpr int end_phase_secs = 5;
};  // namespace PhaseTimes

namespace ScoresConfig {
static constexpr int kill = 2;
// TODO: static constexpr int win = 5;
};  // namespace ScoresConfig

namespace BonificationsConfig {
static constexpr int kill = 100;
// TODO: static constexpr int win = 300;
}  // namespace BonificationsConfig

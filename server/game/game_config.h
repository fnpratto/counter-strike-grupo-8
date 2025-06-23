#pragma once

namespace GameConfig {
static constexpr int tickrate = 30;
static constexpr int player_speed = 4;  // m/s
static constexpr int max_rounds = 10;
};  // namespace GameConfig

namespace PhaseTimes {
static constexpr unsigned int buying_duration = 30;
static constexpr unsigned int round_duration = 120;
static constexpr unsigned int round_end_duration = 5;
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

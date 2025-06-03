#pragma once

namespace GameConfig {
static constexpr int tickrate = 60;
static constexpr int meter_size = 64;                // 1 meter = 64 game world units
static constexpr int player_speed = 2 * meter_size;  // m/s
static constexpr int max_rounds = 10;
static constexpr int max_players = 10;
static constexpr int max_team_players = max_players / 2;
};  // namespace GameConfig

namespace PhaseTimes {
const int buying_phase_secs = 30;
const int playing_phase_secs = 120;
const int round_finished_phase_secs = 5;
};  // namespace PhaseTimes

#pragma once

namespace GameConfig {
const float tickrate = 60;       // ticks per second
const float player_speed = 180;  // pixels per second
const int max_rounds = 10;
const int max_players = 10;
const int max_team_players = max_players / 2;
};  // namespace GameConfig

namespace PhaseTimes {
const int buying_phase_secs = 30;
const int playing_phase_secs = 120;
const int round_finished_phase_secs = 5;
};  // namespace PhaseTimes

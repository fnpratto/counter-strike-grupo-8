#pragma once

namespace GameConfig {
    const float tickrate = 60; // ticks per second
    const float player_speed = 180; // pixels per second
    const int max_rounds = 10;
    const int max_players = 10;
    const int max_team_players = max_players / 2;
};

namespace PhaseTimes {
    const int buying_phase_secs = 30;
    const int playing_phase_secs = 120;
    const int round_finished_phase_secs = 5;
};

namespace PlayerInitialConfig {
    const int initial_money = 800;
    const int full_health = 100;
};

namespace ShopPrices {
    const int price_ak47 = 2700;
    const int price_mag_ak47 = 80;

    const int price_m3 = 1700;
    const int price_mag_m3 = 8;

    const int price_awp = 4750;
    const int price_mag_awp = 125;

    const int price_mag_glock = 20;
};

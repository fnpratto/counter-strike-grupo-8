#pragma once

namespace GlockConfig {
    const int bullets_per_mag = 20;
    const int init_mag_ammo = 20;
    const int init_reserve_ammo = 120;
    const float fire_rate = 6.6;
};

namespace GlockBulletConfig {
    const int min_damage = 20;
    const int max_damage = 30;
    const int speed = 200;
    const float precision = 0.85f; // probability of hitting de target
};

namespace Ak47Config {
    const int burst_bullets = 3;
    const float burst_frec = 0.4f; // bullets per second
    const int bullets_per_mag = 30;
    const int init_mag_ammo = 30;
    const int init_reserve_ammo = 90;
    const float fire_rate = 10;
};

namespace Ak47BulletConfig {
    const int min_damage = 30;
    const int max_damage = 40;
    const int speed = 500;
    const float precision = 0.70f;
};

namespace M3Config {
    const int burst_bullets = 5;
    const float burst_frec = 0;
    const int bullets_per_mag = 1;
    const int init_mag_ammo = 8;
    const int init_reserve_ammo = 32;
    const float fire_rate = 1.1;
    const float cone_max_angle = 30.0f;
};

namespace M3BulletConfig {
    const int min_damage = 8;
    const int max_damage = 12;
    const int speed = 200;
    const float precision = 0.30f;
};

namespace AwpConfig {
    const int bullets_per_mag = 10;
    const int init_mag_ammo = 10;
    const int init_reserve_ammo = 30;
    const float fire_rate = 0.68;
};

namespace AwpBulletConfig {
    const int damage = 100;
    const int speed = 1000;
    const float precision = 1.0f;
};

namespace KnifeConfig {
    const int damage = 50;
};

namespace BombConfig {
    const int damage = 100;
};

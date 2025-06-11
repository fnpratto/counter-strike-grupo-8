#pragma once

#include "server/physics/physics_config.h"

struct GunConfig {
    int bullets_per_mag;
    int init_mag_ammo;
    int init_reserve_ammo;
    float attack_rate;
    float dir_variation_angle;
    int bullets_per_attack;
    float burst_interval;
    int min_damage;
    int max_damage;
    float precision;
    int max_range;
    float falloff;
};

constexpr GunConfig GlockConfig = {.bullets_per_mag = 20,
                                   .init_mag_ammo = 20,
                                   .init_reserve_ammo = 120,
                                   .attack_rate = 6.6,
                                   .dir_variation_angle = 0,
                                   .bullets_per_attack = 1,
                                   .burst_interval = 0,
                                   .min_damage = 40,
                                   .max_damage = 50,
                                   .precision = 0.85f,
                                   .max_range = 10 * PhysicsConfig::meter_size,
                                   .falloff = 0.5f};

constexpr GunConfig Ak47Config = {.bullets_per_mag = 30,
                                  .init_mag_ammo = 30,
                                  .init_reserve_ammo = 90,
                                  .attack_rate = 10,
                                  .dir_variation_angle = 0,
                                  .bullets_per_attack = 3,
                                  .burst_interval = 0.4f,
                                  .min_damage = 30,
                                  .max_damage = 40,
                                  .precision = 0.70f,
                                  .max_range = 20 * PhysicsConfig::meter_size,
                                  .falloff = 0.3f};

constexpr GunConfig M3Config = {.bullets_per_mag = 1,
                                .init_mag_ammo = 8,
                                .init_reserve_ammo = 32,
                                .attack_rate = 1.1,
                                .dir_variation_angle = 30.0f,
                                .bullets_per_attack = 5,
                                .burst_interval = 0,
                                .min_damage = 8,
                                .max_damage = 12,
                                .precision = 0.30f,
                                .max_range = 5 * PhysicsConfig::meter_size,
                                .falloff = 0};

constexpr GunConfig AwpConfig = {.bullets_per_mag = 10,
                                 .init_mag_ammo = 10,
                                 .init_reserve_ammo = 30,
                                 .attack_rate = 0.68,
                                 .dir_variation_angle = 0,
                                 .bullets_per_attack = 1,
                                 .burst_interval = 0,
                                 .min_damage = 100,
                                 .max_damage = 100,
                                 .precision = 1.0f,
                                 .max_range = 100 * PhysicsConfig::meter_size,
                                 .falloff = 0};

namespace KnifeConfig {
const int min_damage = 10;
const int max_damage = 20;
const float attack_rate = 5.0;
const int max_range = 1 * PhysicsConfig::meter_size;
};  // namespace KnifeConfig

namespace BombConfig {
const int damage = 100;
};

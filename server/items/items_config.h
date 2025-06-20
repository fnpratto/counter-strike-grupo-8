#pragma once

#include "common/physics/physics_config.h"

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
    int max_range;
    float precision;
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
                                   .max_range = 10 * PhysicsConfig::meter_size,
                                   .precision = 0.85f,
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
                                  .max_range = 20 * PhysicsConfig::meter_size,
                                  .precision = 0.70f,
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
                                .max_range = 5 * PhysicsConfig::meter_size,
                                .precision = 0.30f,
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
                                 .max_range = 100 * PhysicsConfig::meter_size,
                                 .precision = 1.0f,
                                 .falloff = 0};

namespace KnifeConfig {
static constexpr int min_damage = 10;
static constexpr int max_damage = 20;
static constexpr float attack_rate = 5.0;
static constexpr int max_range = 1 * PhysicsConfig::meter_size;
static constexpr float precision = 1.0f;
static constexpr float falloff = 0;
};  // namespace KnifeConfig

namespace BombConfig {
static constexpr unsigned int secs_to_plant = 3;
static constexpr unsigned int secs_to_explode = 40;
static constexpr unsigned int secs_to_defuse = 5;
static constexpr int damage = 100;
static constexpr int max_range = 15 * PhysicsConfig::meter_size;
static constexpr float precision = 1.0f;
static constexpr float falloff = 0.5f;
};  // namespace BombConfig

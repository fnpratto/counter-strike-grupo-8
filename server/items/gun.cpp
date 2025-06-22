#include "gun.h"

#include <algorithm>
#include <utility>

#include "common/utils/random_float_generator.h"

Gun::Gun(GunType gun, GunConfig gun_config):
        Logic<GunState, GunUpdate>(
                GunState(gun, gun_config.init_mag_ammo, gun_config.init_reserve_ammo)),
        gun_config(gun_config) {}

std::unique_ptr<Gun> Gun::make_gun(const GunType& gun_type) {
    switch (gun_type) {
        case GunType::Glock:
            return std::make_unique<Gun>(GunType::Glock, GlockConfig);
        case GunType::AK47:
            return std::make_unique<Gun>(GunType::AK47, Ak47Config);
        case GunType::AWP:
            return std::make_unique<Gun>(GunType::AWP, AwpConfig);
        case GunType::M3:
            return std::make_unique<Gun>(GunType::M3, M3Config);
        default:
            throw std::invalid_argument("Unknown gun type");
    }
}

bool Gun::is_attacking() const { return state.get_is_attacking(); }

bool Gun::has_ammo() { return state.get_mag_ammo() > 0; }

GunType Gun::get_type() const { return state.get_gun(); }
int Gun::get_mag_ammo() const { return state.get_mag_ammo(); }
int Gun::get_reserve_ammo() const { return state.get_reserve_ammo(); }

void Gun::add_mag() {
    state.set_reserve_ammo(state.get_reserve_ammo() + gun_config.bullets_per_mag);
}

void Gun::start_attacking(TimePoint now) {
    if (!can_attack(gun_config.attack_rate, now))
        return;
    state.set_is_attacking(true);
}

void Gun::reload() {
    int bullets_to_reload = gun_config.bullets_per_mag - state.get_mag_ammo();
    state.set_mag_ammo(state.get_mag_ammo() + bullets_to_reload);
    state.set_reserve_ammo(state.get_reserve_ammo() - bullets_to_reload);
}

std::vector<AttackEffect> Gun::attack(const Vector2D& origin, const Vector2D& dir, TimePoint now) {
    std::vector<AttackEffect> effects;
    if (!is_attacking())
        return effects;

    int bullets = get_bullets_ready_to_fire(now);
    for (int i = 0; i < bullets; i++) {
        int damage = get_random_damage(gun_config.min_damage, gun_config.max_damage);
        Vector2D varied_dir = get_varied_direction(dir, gun_config.dir_variation_angle);

        Effect effect(origin, damage, gun_config.max_range, gun_config.precision,
                      gun_config.falloff);
        effects.push_back(AttackEffect{std::move(effect), varied_dir});

        decrease_mag_ammo();
        burst_bullets_fired++;
    }

    if (!has_ammo() || burst_bullets_fired == gun_config.bullets_per_attack) {
        state.set_is_attacking(false);
        burst_bullets_fired = 0;
    }

    return effects;
}

int Gun::get_bullets_ready_to_fire(TimePoint now) {
    int available_bullets = 0;

    auto burst_interval_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::duration<float>(gun_config.burst_interval));

    int burst_remaining_bullets = gun_config.bullets_per_attack - burst_bullets_fired;
    for (int i = 0; i < burst_remaining_bullets && state.get_mag_ammo(); ++i) {
        auto time_since_last_attack = now - time_last_attack;
        auto required_interval = burst_interval_ns * (i + 1);
        if (time_since_last_attack >= required_interval) {
            available_bullets++;
            time_last_attack = now;
        } else {
            break;
        }
    }

    return available_bullets;
}

Vector2D Gun::get_varied_direction(const Vector2D& dir, float max_angle_deg) {
    RandomFloatGenerator rfg(-max_angle_deg, max_angle_deg);
    float angle_offset_deg = rfg.generate();
    return dir.rotated(angle_offset_deg);
}

void Gun::decrease_mag_ammo() {
    if (state.get_mag_ammo() > 0)
        state.set_mag_ammo(state.get_mag_ammo() - 1);
}

void Gun::reset() {
    time_last_attack = TimePoint::min();
    burst_bullets_fired = 0;
    state.set_is_attacking(false);
}

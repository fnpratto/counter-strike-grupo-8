#pragma once

#include <memory>
#include <vector>

#include "common/models.h"
#include "common/utils/vector_2d.h"
#include "effects/attack_effect.h"
#include "server/clock/clock.h"
#include "server/game/game_config.h"
#include "server/logic.h"
#include "server/states/gun_state.h"

#include "weapon.h"

class Gun: public Logic<GunState, GunUpdate>, public Weapon {
    GameConfig::ItemsConfig::GunConfig gun_config;
    int burst_bullets_fired = 0;

    int get_bullets_ready_to_fire(TimePoint now);
    Vector2D get_varied_direction(const Vector2D& dir, float max_angle_deg);

    void decrease_mag_ammo();

public:
    Gun(GunType gun, const GameConfig::ItemsConfig::GunConfig& initial_config);

    bool is_attacking() const;
    bool has_ammo();

    GunType get_type() const;
    int get_mag_ammo() const;
    int get_reserve_ammo() const;

    void add_mag();

    void start_attacking(TimePoint now);

    void reload();

    std::vector<AttackEffect> attack(const Vector2D& origin, const Vector2D& dir,
                                     TimePoint now) override;

    void reset();
};

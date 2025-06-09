#pragma once

#include <memory>
#include <vector>

#include "common/models.h"
#include "common/utils/vector_2d.h"
#include "server/attack_effects/attack_effect.h"
#include "server/clock/clock.h"
#include "server/logic.h"
#include "server/states/gun_state.h"

#include "weapon.h"

class Gun: public Logic<GunState, GunUpdate>, public Weapon {
    int burst_bullets_fired = 0;

    int get_bullets_ready_to_fire(TimePoint now);

    void decrease_mag_ammo();

public:
    Gun(GunType gun, GunConfig initial_config);

    static std::unique_ptr<Gun> glock();
    static std::unique_ptr<Gun> ak47();
    static std::unique_ptr<Gun> awp();
    static std::unique_ptr<Gun> m3();

    bool has_ammo();

    GunType get_type() const;
    int get_mag_ammo() const;
    int get_reserve_ammo() const;

    void add_mag();

    void start_attacking();

    void reload();

    std::vector<std::unique_ptr<AttackEffect>> attack(Player& player_origin, const Vector2D& dir,
                                                      TimePoint now) override;
};

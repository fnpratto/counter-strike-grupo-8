#pragma once

#include <vector>

#include "common/utils/vector_2d.h"
#include "effects/attack_effect.h"
#include "server/logic.h"
#include "server/states/knife_state.h"

#include "weapon.h"

class Knife: public Logic<KnifeState, KnifeUpdate>, public Weapon {
public:
    Knife();

    bool is_attacking() const;

    void start_attacking();

    std::vector<AttackEffect> attack(const Vector2D& origin, const Vector2D& dir,
                                     TimePoint now) override;

    void stop_attacking();
};

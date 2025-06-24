#pragma once

#include <vector>

#include "common/utils/vector_2d.h"
#include "effects/attack_effect.h"
#include "server/game/game_config.h"
#include "server/logic.h"
#include "server/states/knife_state.h"

#include "weapon.h"

class Knife: public Logic<KnifeState, KnifeUpdate>, public Weapon {
    GameConfig::ItemsConfig::KnifeConfig knife_config;

public:
    explicit Knife(const GameConfig::ItemsConfig::KnifeConfig& knife_config);

    bool is_attacking() const;

    void start_attacking(TimePoint now);

    std::vector<AttackEffect> attack(const Vector2D& origin, const Vector2D& dir,
                                     TimePoint now) override;

    void reset();
};

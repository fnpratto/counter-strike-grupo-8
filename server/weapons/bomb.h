#pragma once

#include <optional>

#include "common/models.h"
#include "common/utils/vector_2d.h"
#include "server/logic.h"
#include "server/states/bomb_state.h"

class Bomb: public Logic<BombState, BombUpdate> {
    int secs_to_explode;

public:
    Bomb();

    Bomb(const Bomb&) = delete;
    Bomb& operator=(const Bomb&) = delete;

    Bomb(Bomb&&) = default;
    Bomb& operator=(Bomb&&) = default;

    int get_secs_to_explode() const;

    void plant();
};

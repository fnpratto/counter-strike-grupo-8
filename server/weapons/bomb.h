#pragma once

#include <optional>

#include "common/models.h"
#include "common/utils/vector_2d.h"
#include "server/logic.h"
#include "server/states/bomb_state.h"

class Bomb: public Logic<BombState, BombUpdate> {
public:
    Bomb(): Logic<BombState, BombUpdate>(BombState()) {}
};

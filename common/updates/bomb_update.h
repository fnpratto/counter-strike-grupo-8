#pragma once

#include <stdexcept>

#include "common/models.h"

#include "state_update.h"

#define BOMB_ATTRS(X, M, V)      \
    X(BombPhaseType, bomb_phase) \
    X(int, secs_to_explode)

DEFINE_UPDATE(BombUpdate, BOMB_ATTRS)

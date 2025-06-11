#pragma once

#include <optional>
#include <stdexcept>

#include "common/utils/vector_2d.h"

#include "state_update.h"

#define BOMB_ATTRS(X, M, U, O, V) O(Vector2D, planted_pos)

DEFINE_UPDATE(BombUpdate, BOMB_ATTRS)

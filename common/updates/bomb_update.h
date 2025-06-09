#pragma once

#include <optional>
#include <stdexcept>

#include "common/utils/vector_2d.h"

#include "state_update.h"

#define BOMB_ATTRS(X, M, U) X(std::optional<Vector2D>, planted_pos)

DEFINE_UPDATE(BombUpdate, BOMB_ATTRS)

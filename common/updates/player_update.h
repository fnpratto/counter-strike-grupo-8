#pragma once

#include <map>
#include <optional>
#include <stdexcept>
#include <string>

#include "common/models.h"
#include "server/utils/vector_2d.h"

#include "inventory_update.h"
#include "state_update.h"

#define PLAYER_ATTRS(X, M, U)     \
    X(Team, team)                 \
    X(Vector2D, pos)              \
    X(Vector2D, aim_direction)    \
    X(Vector2D, velocity)         \
    X(bool, ready)                \
    X(int, health)                \
    X(WeaponSlot, current_weapon) \
    U(InventoryUpdate, inventory) \
    X(bool, is_moving)            \
    X(int, move_dx)               \
    X(int, move_dy)

DEFINE_UPDATE(PlayerUpdate, PLAYER_ATTRS)

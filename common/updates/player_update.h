#pragma once

#include <map>
#include <optional>
#include <stdexcept>
#include <string>

#include "common/models.h"
#include "common/utils/vector_2d.h"

#include "inventory_update.h"
#include "state_update.h"

#define PLAYER_ATTRS(X, M, U, O, V)  \
    X(Team, team)                    \
    X(CharacterType, character_type) \
    X(Vector2D, pos)                 \
    X(int, hitbox_radius)            \
    X(Vector2D, aim_direction)       \
    X(Vector2D, velocity)            \
    X(bool, ready)                   \
    X(int, health)                   \
    X(ItemSlot, equipped_item)       \
    U(InventoryUpdate, inventory)

DEFINE_UPDATE(PlayerUpdate, PLAYER_ATTRS)

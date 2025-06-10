#pragma once

#include <map>
#include <optional>
#include <stdexcept>

#include "common/models.h"

#include "bomb_update.h"
#include "gun_update.h"
#include "knife_update.h"
#include "state_update.h"

#define INVENTORY_ATTRS(X, M, U, O) \
    X(int, money)                   \
    M(ItemSlot, GunUpdate, guns)    \
    U(KnifeUpdate, knife)           \
    O(BombUpdate, bomb)

DEFINE_UPDATE(InventoryUpdate, INVENTORY_ATTRS)

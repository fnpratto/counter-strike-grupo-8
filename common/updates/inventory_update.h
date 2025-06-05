#pragma once

#include <map>
#include <stdexcept>

#include "common/models.h"

#include "gun_update.h"
#include "state_update.h"

#define INVENTORY_ATTRS(X, M, U, V) \
    X(int, money)                   \
    V(ItemSlot, weapons_added)      \
    M(ItemSlot, GunUpdate, guns)

DEFINE_UPDATE(InventoryUpdate, INVENTORY_ATTRS)

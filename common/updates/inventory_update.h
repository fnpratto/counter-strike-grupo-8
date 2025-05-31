#pragma once

#include <map>
#include <stdexcept>

#include "common/models.h"

#include "gun_update.h"
#include "state_update.h"
#include "utility_update.h"

#define INVENTORY_ATTRS(X, M, U)   \
    X(int, money)                  \
    M(WeaponSlot, GunUpdate, guns) \
    M(WeaponSlot, UtilityUpdate, utilities)

DEFINE_UPDATE(InventoryUpdate, INVENTORY_ATTRS)

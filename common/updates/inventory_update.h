#pragma once

#include <map>
#include <optional>
#include <stdexcept>

#include "common/models.h"

#include "bomb_update.h"
#include "gun_update.h"
#include "knife_update.h"
#include "state_update.h"

#define INVENTORY_ATTRS(X, M, V) \
    X(int, money)                \
    M(ItemSlot, GunUpdate, guns) \
    X(KnifeUpdate, knife)        \
    X(std::optional<BombUpdate>, bomb)

DEFINE_UPDATE(InventoryUpdate, INVENTORY_ATTRS)

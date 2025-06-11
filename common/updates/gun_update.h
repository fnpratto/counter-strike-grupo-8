#pragma once

#include <optional>
#include <stdexcept>

#include "common/models.h"

#include "state_update.h"

#define GUN_ATTRS(X, M, U, O, V) \
    X(GunType, gun)              \
    X(int, bullets_per_mag)      \
    X(int, mag_ammo)             \
    X(int, reserve_ammo)         \
    X(bool, is_attacking)

DEFINE_UPDATE(GunUpdate, GUN_ATTRS)

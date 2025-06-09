#pragma once

#include <optional>
#include <stdexcept>

#include "common/models.h"

#include "state_update.h"

#define KNIFE_ATTRS(X, M, U) X(bool, is_attacking)

DEFINE_UPDATE(KnifeUpdate, KNIFE_ATTRS)

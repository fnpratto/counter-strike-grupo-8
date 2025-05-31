#pragma once

#include <map>
#include <optional>
#include <stdexcept>
#include <string>

#include "common/models.h"

#include "state_update.h"

#define UTILITY_ATTRS(X, M, U) X(UtilityType, utility)

DEFINE_UPDATE(UtilityUpdate, UTILITY_ATTRS)

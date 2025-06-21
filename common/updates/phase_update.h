#pragma once

#include <map>
#include <optional>
#include <stdexcept>
#include <string>

#include "common/models.h"

#include "state_update.h"

#define PHASE_ATTRS(X, M, V) \
    X(PhaseType, type)       \
    X(int, secs_remaining)

DEFINE_UPDATE(PhaseUpdate, PHASE_ATTRS)

#pragma once

#include <map>
#include <optional>
#include <stdexcept>
#include <string>

#include "common/models.h"

#include "state_update.h"

#define PHASE_ATTRS(X, M, U, V) \
    X(PhaseType, phase)         \
    X(TimePoint, time)

DEFINE_UPDATE(PhaseUpdate, PHASE_ATTRS)

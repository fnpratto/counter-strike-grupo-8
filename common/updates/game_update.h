#pragma once

#include <map>
#include <optional>
#include <stdexcept>
#include <string>

#include "common/models.h"

#include "phase_update.h"
#include "player_update.h"
#include "state_update.h"

#define GAME_ATTRS(X, M, U, V) \
    U(PhaseUpdate, phase)      \
    X(int, num_rounds)         \
    M(std::string, PlayerUpdate, players)

DEFINE_UPDATE(GameUpdate, GAME_ATTRS)

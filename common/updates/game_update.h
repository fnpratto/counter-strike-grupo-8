#pragma once

#include <map>
#include <optional>
#include <stdexcept>

#include "common/models.h"

#include "phase_update.h"
#include "player_update.h"
#include "state_update.h"

#define GAME_ATTRS(X, M, U) \
    U(PhaseUpdate, phase)   \
    X(int, num_rounds)      \
    X(int, num_tts)         \
    X(int, num_cts)         \
    M(std::string, PlayerUpdate, players)
DEFINE_UPDATE(GameUpdate, GAME_ATTRS)

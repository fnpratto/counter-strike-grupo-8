#pragma once

#include <map>
#include <optional>
#include <stdexcept>
#include <string>

#include "common/game/world_item.h"
#include "common/models.h"
#include "common/utils/vector_2d.h"

#include "phase_update.h"
#include "player_update.h"
#include "state_update.h"

#define GAME_ATTRS(X, M, V)               \
    X(PhaseUpdate, phase)                 \
    X(int, num_rounds)                    \
    M(std::string, PlayerUpdate, players) \
    V(WorldItem<GunType>, dropped_guns)   \
    X(std::optional<WorldItem<BombUpdate>>, bomb)

DEFINE_UPDATE(GameUpdate, GAME_ATTRS)

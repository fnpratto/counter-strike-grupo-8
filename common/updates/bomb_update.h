#pragma once

#include <stdexcept>

#include "state_update.h"

#define BOMB_ATTRS(X, M, U, O, V) X(bool, planted)

DEFINE_UPDATE(BombUpdate, BOMB_ATTRS)

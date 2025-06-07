#pragma once

#include <optional>

#include "common/models.h"
#include "common/utils/vector_2d.h"

class Bomb {
private:
    std::optional<Vector2D> planted_pos;
    // TODO: Time remaining

public:
    Bomb() {}

    bool is_planted() { return planted_pos.has_value(); }
};

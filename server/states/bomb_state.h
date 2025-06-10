#pragma once

#include "common/updates/bomb_update.h"
#include "common/utils/vector_2d.h"

#include "state.h"

class BombState: public State<BombUpdate> {
    std::optional<Vector2D> planted_pos;
    // TODO: Time remaining

public:
    BombState();

    bool is_planted() const;

    BombUpdate get_full_update() const override;  // cppcheck-suppress[virtualCallInConstructor]
};

#pragma once

#include "common/updates/bomb_update.h"

#include "state.h"

class BombState: public State<BombUpdate> {
    bool planted;
    // TODO: Time remaining

public:
    BombState();

    bool is_planted() const;

    BombUpdate get_full_update() const override;  // cppcheck-suppress[virtualCallInConstructor]
};

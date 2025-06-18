#pragma once

#include "common/updates/bomb_update.h"

#include "state.h"

class BombState: public State<BombUpdate> {
    bool planted;

public:
    explicit BombState(bool planted);

    bool is_planted() const;

    void set_planted(bool planted);

    BombUpdate get_full_update() const override;  // cppcheck-suppress[virtualCallInConstructor]
};

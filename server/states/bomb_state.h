#pragma once

#include "common/models.h"
#include "common/updates/bomb_update.h"

#include "state.h"

class BombState: public State<BombUpdate> {
    int secs_to_explode;
    BombPhaseType bomb_phase;

public:
    BombState(int secs_to_explode, BombPhaseType bomb_phase);

    int get_secs_to_explode() const;
    BombPhaseType get_bomb_phase() const;

    void set_secs_to_explode(int secs_to_explode);
    void set_bomb_phase(BombPhaseType bomb_phase);

    BombUpdate get_full_update() const override;  // cppcheck-suppress[virtualCallInConstructor]
};

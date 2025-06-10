#pragma once

#include "common/models.h"
#include "common/updates/phase_update.h"

#include "state.h"

class PhaseState: public State<PhaseUpdate> {
    PhaseType phase;
    int secs_remaining;

public:
    explicit PhaseState(PhaseType phase_type);

    PhaseType get_phase() const;
    int get_secs_remaining() const;

    void set_phase(PhaseType new_phase);
    void set_secs_remaining(int new_secs_remaining);

    PhaseUpdate get_full_update() const override;  // cppcheck-suppress[virtualCallInConstructor]
};

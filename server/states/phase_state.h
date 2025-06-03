#pragma once

#include "common/models.h"
#include "common/updates/phase_update.h"
#include "server/clock/clock.h"

#include "state.h"

class PhaseState: public State<PhaseUpdate> {
    PhaseType phase;
    TimePoint time;

public:
    PhaseState(PhaseType phase_type, TimePoint time_point);

    PhaseType get_phase() const;
    TimePoint get_time() const;

    void set_phase(PhaseType new_phase);
    void set_time(TimePoint new_time);

    PhaseUpdate get_full_update() const override;  // cppcheck-suppress[virtualCallInConstructor]
};

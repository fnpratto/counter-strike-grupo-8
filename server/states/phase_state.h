#pragma once

#include "common/models.h"
#include "common/updates/phase_update.h"
#include "server/clock/clock.h"

#include "state.h"

class PhaseState: public State<PhaseUpdate> {
    PhaseType phase;
    TimePoint time;

public:
    PhaseState(PhaseType phase_type, TimePoint time_point): phase(phase_type), time(time_point) {
        updates = get_full_update();
    }

    PhaseType get_phase() const { return phase; }
    TimePoint get_time() const { return time; }

    void set_phase(PhaseType new_phase) {
        phase = new_phase;
        updates.set_phase(new_phase);
    }
    void set_time(TimePoint new_time) {
        if (time == new_time)
            return;
        time = new_time;
        updates.set_time(new_time);
    }

    PhaseUpdate get_full_update() const override {
        PhaseUpdate full_update;
        full_update.set_phase(phase);
        full_update.set_time(time);
        return full_update;
    }
};

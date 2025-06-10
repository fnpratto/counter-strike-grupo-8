#include "phase_state.h"

PhaseState::PhaseState(PhaseType phase_type, TimePoint time_point):
        phase(phase_type), time(time_point) {
    updates = get_full_update();
}

PhaseType PhaseState::get_phase() const { return phase; }
TimePoint PhaseState::get_time() const { return time; }

void PhaseState::set_phase(PhaseType new_phase) {
    if (phase == new_phase)
        return;
    phase = new_phase;
    updates.set_phase(new_phase);
}
void PhaseState::set_time(TimePoint new_time) {
    if (time == new_time)
        return;
    time = new_time;
    updates.set_time(new_time);
}

PhaseUpdate PhaseState::get_full_update() const {
    PhaseUpdate full_update;
    full_update.set_phase(phase);
    full_update.set_time(time);
    return full_update;
}

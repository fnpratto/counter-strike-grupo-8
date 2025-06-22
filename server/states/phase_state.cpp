#include "phase_state.h"

PhaseState::PhaseState(PhaseType phase_type): phase(phase_type), secs_remaining(0) {
    updates = get_full_update();
}

PhaseType PhaseState::get_phase() const { return phase; }
int PhaseState::get_secs_remaining() const { return secs_remaining; }

void PhaseState::set_phase(PhaseType new_phase) {
    if (phase == new_phase)
        return;
    phase = new_phase;
    updates.set_type(new_phase);
}
void PhaseState::set_secs_remaining(int new_secs_remaining) {
    if (secs_remaining == new_secs_remaining)
        return;
    if (new_secs_remaining < 0)
        secs_remaining = 0;
    else
        secs_remaining = new_secs_remaining;
    updates.set_secs_remaining(new_secs_remaining);
}

PhaseUpdate PhaseState::get_full_update() const {
    PhaseUpdate full_update;
    full_update.set_type(phase);
    full_update.set_secs_remaining(secs_remaining);
    return full_update;
}

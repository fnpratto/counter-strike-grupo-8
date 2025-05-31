#pragma once

#include <utility>

#include "states/state.h"

template <typename State, typename Update>
class Logic {
protected:
    State state;

public:
    explicit Logic(State&& state): state(std::move(state)) {}

    Update get_updates() const { return state.get_updates(); }
    void clear_updates() { state.clear_updates(); }
    Update get_full_update() const { return state.get_full_update(); }
};

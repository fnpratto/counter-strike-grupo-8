#pragma once

template <typename Update>
class State {
protected:
    Update updates;

public:
    State() {}

    virtual Update get_updates() const { return updates; }
    virtual Update get_full_update() const = 0;
    virtual void clear_updates() { updates.clear(); }

    State(const State&) = delete;
    State& operator=(const State&) = delete;

    State(State&&) = default;
    State& operator=(State&&) = default;

    virtual ~State() = default;
};

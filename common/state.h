#pragma once

template <typename Update>
class State {
protected:
    Update updates;

public:
    Update get_updates() const { return updates; }
    virtual Update get_full_update() const = 0;

    void clear_updates() { updates.clear(); }

    virtual ~State() = default;
};

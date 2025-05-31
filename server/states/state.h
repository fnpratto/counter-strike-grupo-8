#pragma once

template <typename Update>
class State {
protected:
    Update updates;

public:
    virtual Update get_updates() const { return updates; }
    virtual Update get_full_update() const = 0;
    virtual void clear_updates() { updates.clear(); }

    virtual ~State() = default;
};

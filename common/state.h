#pragma once

template <typename Update>
class State {
protected:
    Update updates;

public:
    virtual Update get_updates() const { return updates; }
    virtual void clear_updates() { updates.clear(); }
};

#pragma once

#include "common/models.h"
#include "common/updates/knife_update.h"

#include "state.h"

class KnifeState: public State<KnifeUpdate> {
private:
    bool is_attacking;

public:
    KnifeState();

    bool get_is_attacking() const;

    void set_is_attacking(bool is_attacking);

    KnifeUpdate get_full_update() const override;  // cppcheck-suppress[virtualCallInConstructor]
};

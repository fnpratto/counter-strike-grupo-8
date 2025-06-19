#pragma once

#include <memory>

#include "common/models.h"
#include "common/utils/vector_2d.h"
#include "server/clock/clock.h"

class Player;

class PlayerStatus {
public:
    virtual void handle_start_moving([[maybe_unused]] Player& player,
                                     [[maybe_unused]] const Vector2D& velocity) {}
    virtual void handle_stop_moving([[maybe_unused]] Player& player) {}
    virtual void handle_start_attacking([[maybe_unused]] Player& player) {}
    virtual void handle_switch_item([[maybe_unused]] Player& player,
                                    [[maybe_unused]] ItemSlot slot) {}
    virtual void handle_start_planting([[maybe_unused]] Player& player,
                                       [[maybe_unused]] TimePoint now) {}
    virtual void handle_stop_planting([[maybe_unused]] Player& player,
                                      [[maybe_unused]] TimePoint now) {}

    virtual ~PlayerStatus() = default;
};

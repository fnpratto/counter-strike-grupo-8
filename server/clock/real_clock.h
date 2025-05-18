#pragma once

#include "clock.h"

class RealClock : public Clock {
public:
    TimePoint now() const override {
        return std::chrono::steady_clock::now();
    }
};

#pragma once

#include "clock.h"

class MockClock : public Clock {
private:
    std::chrono::steady_clock::time_point current_time;

public:
    explicit MockClock(std::chrono::steady_clock::time_point start_time) : 
            current_time(start_time) {}

    void advance(std::chrono::seconds secs) { current_time += secs; }

    std::chrono::steady_clock::time_point now() const override {
        return current_time;
    }
};

#pragma once

#include "clock.h"

class MockClock : public Clock {
private:
    TimePoint current_time;

public:
    explicit MockClock(TimePoint start_time) : 
            current_time(start_time) {}

    void advance(std::chrono::seconds secs) { current_time += secs; }

    TimePoint now() const override {
        return current_time;
    }
};

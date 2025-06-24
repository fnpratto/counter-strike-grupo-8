#pragma once

#include "server/clock/clock.h"

class MockClock: public Clock {
private:
    TimePoint current_time;

public:
    explicit MockClock(TimePoint start_time): current_time(start_time) {}

    void advance(std::chrono::duration<float> secs) {
        auto duration = std::chrono::duration_cast<std::chrono::steady_clock::duration>(
                std::chrono::duration<float>(secs));
        current_time += duration;
    }

    TimePoint now() const override { return current_time; }
};

#pragma once

#include <chrono>

using TimePoint = std::chrono::steady_clock::time_point;

class Clock {
public:
    virtual TimePoint now() const = 0;
    virtual ~Clock() = default;
};

#pragma once

#include <chrono>

class Clock {
public:
    virtual std::chrono::steady_clock::time_point now() const = 0;
    virtual ~Clock() = default;
};

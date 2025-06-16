#include "rate_controller.h"

#include <thread>

RateController::RateController(int rate, int max_lag_frames):
        target_rate(rate),
        frame_duration(1000 / rate),
        next_frame_time(std::chrono::steady_clock::now()),
        max_lag_frames(max_lag_frames) {}

void RateController::run_at_rate(std::function<bool()> callback) {
    while (true) {
        auto now = std::chrono::steady_clock::now();

        if (now >= next_frame_time) {
            // We're at or past the time for the next execution
            // Call callback(), if it returns false, exit loop
            if (!callback())
                break;

            // Schedule next execution (even if we're late)
            next_frame_time += frame_duration;

            // If we are significantly behind, skip ahead to avoid lag accumulation
            auto max_lag = max_lag_frames * frame_duration;
            if (now - next_frame_time > max_lag) {
                next_frame_time = now;
            }
        } else {
            // We're ahead of schedule: sleep to limit execution rate
            std::this_thread::sleep_until(next_frame_time);
        }
    }
}

bool RateController::should_run() {
    auto now = std::chrono::steady_clock::now();

    if (now < next_frame_time)
        return false;  // Not time to run yet

    // We're at or past the time for the next execution

    // Schedule next execution (even if we're late)
    next_frame_time += frame_duration;

    // If we are significantly behind, skip ahead to avoid lag accumulation
    auto max_lag = max_lag_frames * frame_duration;
    if (now - next_frame_time > max_lag) {
        next_frame_time = now;
    }

    return true;
}

void RateController::set_rate(int rate) {
    target_rate = rate;
    frame_duration = std::chrono::milliseconds(1000 / rate);
}

int RateController::get_rate() const { return target_rate; }

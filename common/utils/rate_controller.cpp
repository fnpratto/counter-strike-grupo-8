#include "rate_controller.h"

#include <iomanip>
#include <iostream>
#include <thread>

RateController::RateController(int rate, int max_lag_frames):
        target_rate(rate),
        frame_duration(1000 / rate),
        next_frame_time(std::chrono::steady_clock::now()),
        max_lag_frames(max_lag_frames),
        last_frame_time(std::chrono::steady_clock::now()),
        debug_start_time(std::chrono::steady_clock::now()) {}

void RateController::run_at_rate(std::function<bool()> callback) {
    if (debug_mode) {
        debug_start_time = std::chrono::steady_clock::now();
        frame_count = 0;
        skipped_frames = 0;
        std::cout << "[DEBUG] Starting rate controller at " << target_rate << " Hz" << std::endl;
    }

    while (true) {
        auto now = std::chrono::steady_clock::now();

        if (now >= next_frame_time) {
            // Debug: Track frame time
            if (debug_mode) {
                auto frame_time = std::chrono::duration_cast<std::chrono::microseconds>(
                        now - last_frame_time);
                frame_count++;

                // Print debug info every second
                auto elapsed =
                        std::chrono::duration_cast<std::chrono::seconds>(now - debug_start_time);
                if (elapsed.count() > 0 && frame_count % target_rate == 0) {
                    double actual_fps = static_cast<double>(frame_count) / elapsed.count();
                    auto frame_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                            now - last_frame_time);
                    std::cout << "[DEBUG] FPS: " << std::fixed << std::setprecision(2) << actual_fps
                              << " | Frame time: " << frame_time_ms.count() << "ms"
                              << " | Skipped frames: " << skipped_frames << std::endl;
                }

                last_frame_time = now;
            }

            // We're at or past the time for the next execution
            // Call callback(), if it returns false, exit loop
            if (!callback())
                break;

            // Schedule next execution (even if we're late)
            next_frame_time += frame_duration;

            // If we are significantly behind, skip ahead to avoid lag accumulation
            auto max_lag = max_lag_frames * frame_duration;
            if (now - next_frame_time > max_lag) {
                if (debug_mode) {
                    auto frames_behind = std::chrono::duration_cast<std::chrono::milliseconds>(
                                                 now - next_frame_time)
                                                 .count() /
                                         frame_duration.count();
                    std::cout << "[DEBUG] WARNING: Skipping " << frames_behind
                              << " frames due to lag" << std::endl;
                    skipped_frames += frames_behind;
                }
                next_frame_time = now;
            }
        } else {
            // We're ahead of schedule: sleep to limit execution rate
            std::this_thread::sleep_until(next_frame_time);
        }
    }

    if (debug_mode) {
        auto total_time = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - debug_start_time);
        double avg_fps = static_cast<double>(frame_count) / total_time.count();
        std::cout << "[DEBUG] Session ended - Avg FPS: " << std::fixed << std::setprecision(2)
                  << avg_fps << " | Total frames: " << frame_count
                  << " | Total skipped: " << skipped_frames << std::endl;
    }
}

bool RateController::should_run() {
    auto now = std::chrono::steady_clock::now();

    if (now < next_frame_time)
        return false;  // Not time to run yet

    // We're at or past the time for the next execution

    // Debug: Track frame time
    if (debug_mode) {
        auto frame_time =
                std::chrono::duration_cast<std::chrono::microseconds>(now - last_frame_time);
        frame_count++;

        // Print debug info every second
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - debug_start_time);
        if (elapsed.count() > 0 && frame_count % target_rate == 0) {
            double actual_fps = static_cast<double>(frame_count) / elapsed.count();
            std::cout << "[DEBUG] FPS: " << std::fixed << std::setprecision(2) << actual_fps
                      << " | Frame time: " << frame_time.count() << "μs"
                      << " | Skipped frames: " << skipped_frames << std::endl;
        }

        last_frame_time = now;
    }

    // Schedule next execution (even if we're late)
    next_frame_time += frame_duration;

    // If we are significantly behind, skip ahead to avoid lag accumulation
    auto max_lag = max_lag_frames * frame_duration;
    if (now - next_frame_time > max_lag) {
        if (debug_mode) {
            auto frames_behind =
                    std::chrono::duration_cast<std::chrono::milliseconds>(now - next_frame_time)
                            .count() /
                    frame_duration.count();
            std::cout << "[DEBUG] WARNING: Skipping " << frames_behind << " frames due to lag"
                      << std::endl;
            skipped_frames += frames_behind;
        }
        next_frame_time = now;
    }

    return true;
}

void RateController::set_rate(int rate) {
    target_rate = rate;
    frame_duration = std::chrono::milliseconds(1000 / rate);

    // Reset debug counters when rate changes
    if (debug_mode) {
        debug_start_time = std::chrono::steady_clock::now();
        frame_count = 0;
        skipped_frames = 0;
        std::cout << "[DEBUG] Rate changed to " << target_rate << " Hz" << std::endl;
    }
}

int RateController::get_rate() const { return target_rate; }

void RateController::set_debug_mode(bool enabled) {
    debug_mode = enabled;
    if (enabled) {
        debug_start_time = std::chrono::steady_clock::now();
        last_frame_time = debug_start_time;
        frame_count = 0;
        skipped_frames = 0;
        std::cout << "[DEBUG] Debug mode enabled for rate controller (" << target_rate << " Hz)"
                  << std::endl;
    } else {
        std::cout << "[DEBUG] Debug mode disabled" << std::endl;
    }
}

bool RateController::is_debug_enabled() const { return debug_mode; }

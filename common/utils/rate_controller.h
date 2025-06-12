#ifndef RATE_CONTROLLER_H
#define RATE_CONTROLLER_H

#include <chrono>
#include <functional>

/**
 * @brief A utility class for controlling execution rate (FPS/TPS).
 *
 * This class provides functionality to execute a callback function at a
 * controlled rate, handling timing, lag compensation, and frame skipping.
 */
class RateController {
private:
    int target_rate;
    std::chrono::milliseconds frame_duration;
    std::chrono::steady_clock::time_point next_frame_time;
    int max_lag_frames;

    bool is_blocking = false;

public:
    /**
     * @brief Constructs a RateController with the specified target rate.
     *
     * @param rate Target execution rate (e.g., 60 for 60 FPS, 128 for 128 TPS)
     * @param max_lag_frames Maximum frames to lag before skipping ahead (default: 5)
     */
    explicit RateController(int rate, int max_lag_frames = 5);

    /**
     * @brief Executes the callback function at the controlled rate.
     *
     * This method runs a loop that calls the provided callback function at the
     * target rate. The callback should return true to continue execution or
     * false to stop the loop.
     *
     * @param callback Function to execute at the controlled rate
     */
    void run_at_rate(std::function<bool()> callback);

    void set_blocking(bool blocking) { is_blocking = blocking; }

    /**
     * @brief Sets a new target rate.
     *
     * @param rate New target rate
     */
    void set_rate(int rate);

    /**
     * @brief Gets the current target rate.
     *
     * @return Current target rate
     */
    int get_rate() const;

    bool should_run();
};

#endif  // RATE_CONTROLLER_H

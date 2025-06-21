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

    // Debug mode variables
    bool debug_mode = false;
    std::chrono::steady_clock::time_point last_frame_time;
    std::chrono::steady_clock::time_point debug_start_time;
    int frame_count = 0;
    int skipped_frames = 0;

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

    /**
     * @brief Enables or disables debug mode.
     *
     * When debug mode is enabled, the controller will output frame rate,
     * frame time, and warnings about skipped frames to the console.
     *
     * @param enabled True to enable debug mode, false to disable
     */
    void set_debug_mode(bool enabled);

    /**
     * @brief Checks if debug mode is currently enabled.
     *
     * @return True if debug mode is enabled, false otherwise
     */
    bool is_debug_enabled() const;
};

#endif  // RATE_CONTROLLER_H

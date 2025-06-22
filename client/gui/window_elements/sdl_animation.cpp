#include "sdl_animation.h"

#include <algorithm>

SdlAnimation::SdlAnimation(const SdlWindow& window, const std::string& path,
                           const std::vector<SDL_Rect>& clips, std::chrono::milliseconds duration,
                           bool repeats):
        texture(path, window, 32, 32),
        clips(clips),
        animation_duration(duration),
        repeats(repeats),
        finished(false) {
    if (clips.empty())
        throw std::runtime_error("No animation clips provided for SdlAnimation.");
    if (duration.count() <= 0)
        throw std::runtime_error("Animation duration must be positive.");
    start_time = std::chrono::steady_clock::now();
}

void SdlAnimation::reset() {
    start_time = std::chrono::steady_clock::now();
    finished = false;
}

bool SdlAnimation::is_finished() const { return finished; }

void SdlAnimation::render(int x, int y, double angle) {
    if (finished && !repeats) {
        // If animation is finished and doesn't repeat, render the last frame
        texture.render(x, y, &clips[clips.size() - 1], angle, nullptr, SDL_FLIP_NONE);
        return;
    }

    auto current_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);

    if (!repeats && elapsed >= animation_duration) {
        // Animation finished and doesn't repeat
        finished = true;
        texture.render(x, y, &clips[clips.size() - 1], angle, nullptr, SDL_FLIP_NONE);
        return;
    }

    // Calculate current frame based on elapsed time
    auto time_per_frame = animation_duration / clips.size();
    int frame_index;

    if (repeats) {
        // For repeating animations, use modulo to loop
        auto cycle_time = elapsed % animation_duration;
        frame_index = static_cast<int>(cycle_time / time_per_frame);
    } else {
        // For non-repeating animations, clamp to last frame
        frame_index = static_cast<int>(elapsed / time_per_frame);
        frame_index = std::min(frame_index, static_cast<int>(clips.size() - 1));
    }

    // Ensure frame_index is within bounds
    frame_index = std::max(0, std::min(frame_index, static_cast<int>(clips.size() - 1)));

    texture.render(x, y, &clips[frame_index], angle, nullptr, SDL_FLIP_NONE);
}

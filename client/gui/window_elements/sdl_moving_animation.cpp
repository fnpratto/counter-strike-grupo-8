#include "sdl_moving_animation.h"

#include <algorithm>

SdlMovingAnimation::SdlMovingAnimation(const SdlWindow& window, const std::string& path, int width,
                                       int height, std::chrono::milliseconds duration,
                                       bool repeats):
        texture(std::make_unique<SdlTexture>(path, window, width, height)),
        animation_duration(duration),
        repeats(repeats),
        finished(false),
        progress(0.0f) {
    if (duration.count() <= 0)
        throw std::runtime_error("Animation duration must be positive.");
    start_time = std::chrono::steady_clock::now();
}

void SdlMovingAnimation::reset() {
    start_time = std::chrono::steady_clock::now();
    finished = false;
    progress = 0.0f;
}

bool SdlMovingAnimation::is_finished() const { return finished; }

float SdlMovingAnimation::get_progress() const { return progress; }

void SdlMovingAnimation::render(const Vector2D& origin, const Vector2D& destination, double angle) {
    auto current_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);

    // Calculate progress (0.0 to 1.0)
    if (animation_duration.count() > 0) {
        progress = static_cast<float>(elapsed.count()) /
                   static_cast<float>(animation_duration.count());
    } else {
        progress = 1.0f;
    }

    if (!repeats && progress >= 1.0f) {
        // Animation finished and doesn't repeat
        finished = true;
        progress = 1.0f;
    } else if (repeats && progress >= 1.0f) {
        // For repeating animations, use modulo to loop
        progress = progress - static_cast<int>(progress);
    }

    // Clamp progress to [0.0, 1.0] range
    progress = std::max(0.0f, std::min(progress, 1.0f));

    // Calculate current position based on progress
    float current_x = origin.get_x() + (destination.get_x() - origin.get_x()) * progress;
    float current_y = origin.get_y() + (destination.get_y() - origin.get_y()) * progress;

    texture->render(static_cast<int>(current_x), static_cast<int>(current_y), angle);
}

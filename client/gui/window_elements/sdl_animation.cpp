#include "sdl_animation.h"

SdlAnimation::SdlAnimation(const SdlWindow& window, const std::string& path,
                           const std::vector<SDL_Rect>& clips):
        texture(path, window, 32, 32), clips(clips) {
    animation_frame = 0;
}

void SdlAnimation::reset() { animation_frame = 0; }

// TODO animations should be independent of the framerate
void SdlAnimation::render(int x, int y, double angle) {

    if (clips.empty()) {
        throw std::runtime_error("No animation clips provided for SdlAnimation.");
    }

    // Render the current animation frame
    texture.render(x, y, &clips[animation_frame], angle, nullptr, SDL_FLIP_NONE);

    // Advance to the next frame in the animation
    animation_frame = (animation_frame + 1) % clips.size();
}

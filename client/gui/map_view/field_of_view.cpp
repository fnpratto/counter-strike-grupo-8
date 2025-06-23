#include "field_of_view.h"

#include <cmath>


FieldOfView::FieldOfView(SdlWindow& window, float range):
        window(window), range(range), mask("../assets/black_square.png", window) {}

bool FieldOfView::is_visible(const Vector2D& camera_pos, const Vector2D& target_pos) const {
    float dx = target_pos.get_x() - camera_pos.get_x();
    float dy = target_pos.get_y() - camera_pos.get_y();
    float distance = std::sqrt(dx * dx + dy * dy);

    return distance <= range;
}

void FieldOfView::render(int screen_width, int screen_height, float angle) {
    SDL_Renderer* renderer = window.getRenderer();

    SDL_Point center = {screen_width / 2, screen_height / 2};

    SDL_Rect dst_rect = {0, 0, 4900, 3800};

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    mask.render(0, 0, screen_width, screen_height, &dst_rect, angle,
                &center,  // Rotation center relative to the texture
                SDL_FLIP_NONE);
}

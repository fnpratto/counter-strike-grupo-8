#include "field_of_view.h"

#include <cmath>


FieldOfView::FieldOfView(const SdlWindow& window, float range):
        window(window), range(range), mask("../assets/black_square.png", window) {}

bool FieldOfView::is_visible(const Vector2D& camera_pos, const Vector2D& target_pos) const {
    float dx = target_pos.get_x() - camera_pos.get_x();
    float dy = target_pos.get_y() - camera_pos.get_y();
    float distance = std::sqrt(dx * dx + dy * dy);

    return distance <= range;
}

void FieldOfView::render(int screen_width, int screen_height, float angle) {

    SDL_Point center_w = {screen_width / 2, screen_height / 2};

    // Calculate diagonal to ensure full coverage during rotation
    int diagonal = static_cast<int>(
            std::ceil(std::sqrt(screen_width * screen_width + screen_height * screen_height)));

    SDL_Rect dst_rect = {0, 0, 1000, 1000};
    SDL_Point center = {diagonal / 2, diagonal / 2};

    mask.render(center_w.x - diagonal / 2, center_w.y - diagonal / 2, diagonal, diagonal, &dst_rect,
                angle, &center, SDL_FLIP_NONE);
}

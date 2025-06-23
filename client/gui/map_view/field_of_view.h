#pragma once
#include "client/gui/window_elements/sdl_texture.h"
#include "client/gui/window_elements/sdl_window.h"
#include "common/map/tile.h"
#include "common/physics/physics_config.h"
#include "common/updates/player_update.h"

class FieldOfView {
private:
    SdlWindow& window;
    float range;
    SdlTexture mask;

public:
    FieldOfView(SdlWindow& window, float range);
    bool is_visible(const Vector2D& camera_pos, const Vector2D& target_pos) const;
    void render(int screen_width, int screen_height, float angle);
};

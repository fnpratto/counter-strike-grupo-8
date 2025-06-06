#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include "../window_elements/area.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/updates/game_update.h"

class Camera {
private:
    int screen_width;
    int screen_height;
    int world_width;
    int world_height;
    Vector2D offset;

public:
    Camera(int screen_width, int screen_height, int world_width, int world_height);
    void center(const Vector2D& target_pos);
    Vector2D get_offset() const;
    ~Camera();
};


#endif  // CAMERA_H

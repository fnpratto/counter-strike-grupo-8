#pragma once

#include <utility>

#include "common/utils/vector_2d.h"

class Box {
private:
    Vector2D pos;

public:
    explicit Box(Vector2D&& pos): pos(std::move(pos)) {}

    Vector2D get_pos() const { return pos; }

    bool is_collidable() const { return true; }
};

#pragma once

#include <memory>

#include "common/utils/rect_bounds.h"

template <typename T>
struct WorldItem {
    T item;
    RectBounds hitbox_bounds;
};

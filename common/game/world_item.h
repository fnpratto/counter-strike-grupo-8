#pragma once

#include <memory>

#include "common/utils/rectangle.h"

template <typename T>
struct WorldItem {
    T item;
    Rectangle hitbox;
};

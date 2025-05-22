#pragma once

#include "server/utils/vector_2d.h"

class Bullet {
private:
    Vector2D origin;
    const Vector2D& dir;
    int damage;
    float precision;
    float delay;

public:
    Bullet(const Vector2D& origin, const Vector2D& dir, 
            int damage, float precision, float delay) : 
                origin(origin),
                dir(dir),
                damage(damage),
                precision(precision),
                delay(delay) {}
};

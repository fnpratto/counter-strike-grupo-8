#pragma once

#include "common/utils/vector_2d.h"

struct Tile {
    Vector2D pos;
    int id;
    bool is_collidable;
    bool is_spawn_tt;
    bool is_spawn_ct;
    bool is_bomb_site;
};

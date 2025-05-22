#pragma once

#include "common/models.h"
#include "server/utils/vector_2d.h"

class Tile {
private:
    MapTileType tile;
    Vector2D pos;

public:
    Tile(MapTileType tile) : tile(tile) {}

    Vector2D get_pos() const { return pos; }
    
    bool is_collidable() const {
        if (tile == MapTileType::Floor)
            return false;
        return true;
    }
};

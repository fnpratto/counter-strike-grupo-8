#pragma once

#include <utility>

#include "common/models.h"
#include "common/utils/vector_2d.h"

class Tile {
private:
    MapTileType tile;
    Vector2D pos;

public:
    Tile(MapTileType tile, Vector2D&& pos): tile(tile), pos(std::move(pos)) {}

    Vector2D get_pos() const { return pos; }

    bool is_collidable() const {
        if (tile == MapTileType::Floor)
            return false;
        return true;
    }
};

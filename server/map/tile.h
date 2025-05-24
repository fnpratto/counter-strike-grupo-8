#pragma once

#include <utility>

#include "common/models.h"
#include "server/utils/vector_2d.h"

class Tile {
private:
    MapTileType tile;
    Vector2D grid_pos;

public:
    Tile(MapTileType tile, Vector2D&& grid_pos): tile(tile), grid_pos(std::move(grid_pos)) {}

    Vector2D get_grid_pos() const { return grid_pos; }

    bool is_collidable() const {
        if (tile == MapTileType::Floor)
            return false;
        return true;
    }
};

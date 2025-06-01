#pragma once

#include <string>
#include <vector>

#include "common/utils/vector_2d.h"

#include "tile.h"

class Map {
private:
    std::string name;
    int tile_size;
    std::vector<Tile> tiles;
    std::vector<Vector2D> spawns_tts;
    std::vector<Vector2D> spawns_cts;
    std::vector<Vector2D> bomb_sites;

    Vector2D random_spawn_pos(const std::vector<Vector2D>& spawns) const;

public:
    Map(const std::string& name, int tile_size);

    void validate() const;

    int get_tile_size() const;

    void add_tile(MapTileType type, Vector2D&& grid_pos);
    void add_spawn_tt(Vector2D&& grid_pos);
    void add_spawn_ct(Vector2D&& grid_pos);
    void add_bomb_site(Vector2D&& grid_pos);

    Vector2D random_spawn_tt_pos() const;
    Vector2D random_spawn_ct_pos() const;

    ~Map();
};

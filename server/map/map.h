#pragma once

#include <string>
#include <vector>

#include "server/utils/vector_2d.h"

#include "tile.h"

class Map {
private:
    std::string name;
    std::vector<Tile> tiles;
    std::vector<Vector2D> spawns_tts;
    std::vector<Vector2D> spawns_cts;
    std::vector<Vector2D> bomb_sites;

    bool is_pos_in_vector(const std::vector<Vector2D>& vector, const Vector2D& pos);

    Vector2D random_spawn_pos(const std::vector<Vector2D>& spawns) const;

public:
    explicit Map(const std::string& name);

    bool is_collidable(const Vector2D& pos);
    bool is_spawn_tt(const Vector2D& pos);
    bool is_spawn_ct(const Vector2D& pos);
    bool is_bomb_site(const Vector2D& pos);

    void add_tile(Tile&& tile);
    void add_spawn_tt(Vector2D&& pos);
    void add_spawn_ct(Vector2D&& pos);
    void add_bomb_site(Vector2D&& pos);

    Vector2D random_spawn_tt_pos() const;
    Vector2D random_spawn_ct_pos() const;

    ~Map();
};

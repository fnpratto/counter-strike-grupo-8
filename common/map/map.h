#pragma once

#include <string>
#include <vector>

#include "common/utils/vector_2d.h"

#include "tile.h"

class Map {
private:
    std::string name;
    int max_players;
    // TODO: split tiles vector in walls, boxes and floor?
    //       we can split un 2: collidable tiles and non-collidable
    std::vector<Tile> tiles;
    std::vector<Vector2D> spawns_tts;
    std::vector<Vector2D> spawns_cts;
    std::vector<Vector2D> bomb_sites;

    Vector2D random_spawn_pos(const std::vector<Vector2D>& spawns) const;

    bool is_pos_in_vector(const Vector2D& pos, const std::vector<Vector2D>& vector) const;

public:
    Map(const std::string& name, int max_players);

    void validate() const;

    int get_max_players() const;
    const std::vector<Tile>& get_tiles() const;

    void add_tile(MapTileType type, Vector2D&& pos);
    void add_spawn_tt(Vector2D&& pos);
    void add_spawn_ct(Vector2D&& pos);
    void add_bomb_site(Vector2D&& pos);

    Vector2D random_spawn_tt_pos() const;
    Vector2D random_spawn_ct_pos() const;

    bool is_spawn_tt_pos(const Vector2D& pos) const;
    bool is_spawn_ct_pos(const Vector2D& pos) const;

    ~Map();
};

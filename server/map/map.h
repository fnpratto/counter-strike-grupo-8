#pragma once

#include <string>
#include <vector>

#include "common/utils/vector_2d.h"

#include "box.h"
#include "floor.h"
#include "wall.h"

class Map {
private:
    std::string name;
    int max_players;
    std::vector<Floor> floors;
    std::vector<Wall> walls;
    std::vector<Box> boxes;
    std::vector<Vector2D> spawns_tts;
    std::vector<Vector2D> spawns_cts;
    std::vector<Vector2D> bomb_sites;

    Vector2D random_spawn_pos(const std::vector<Vector2D>& spawns) const;

    bool is_pos_in_vector(const Vector2D& pos, const std::vector<Vector2D>& vector) const;

public:
    Map(const std::string& name, int max_players);

    void validate() const;

    int get_max_players() const;
    const std::vector<Floor>& get_floors() const;
    const std::vector<Wall>& get_walls() const;
    const std::vector<Box>& get_boxes() const;

    void add_floor(Vector2D&& pos);
    void add_wall(Vector2D&& pos);
    void add_box(Vector2D&& pos);
    void add_spawn_tt(Vector2D&& pos);
    void add_spawn_ct(Vector2D&& pos);
    void add_bomb_site(Vector2D&& pos);

    Vector2D random_spawn_tt_pos() const;
    Vector2D random_spawn_ct_pos() const;

    bool is_spawn_tt_pos(const Vector2D& pos) const;
    bool is_spawn_ct_pos(const Vector2D& pos) const;

    ~Map();
};

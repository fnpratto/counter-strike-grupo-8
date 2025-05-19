#pragma once

#include <string>
#include <vector>
#include <map>

#include "common/models.h"
#include "server/utils/vector_2d.h"
#include "server/weapons/bullet.h"
#include "server/weapons/knife.h"
#include "server/weapons/bomb.h"

class Map {
private:
    std::string name;
    std::vector<std::vector<MapTileType>> tiles;
    std::vector<Vector2D> spawns_tts;
    std::vector<Vector2D> spawns_cts;
    std::vector<Vector2D> bomb_sites;
    std::map<std::string, Vector2D> players;
    std::vector<Bullet> bullets;

    bool is_pos_in_vector(const std::vector<Vector2D>& vector, const Vector2D& pos);

public:
    explicit Map(std::string&& name);

    bool is_collidable(const Vector2D& pos);
    bool is_in_spawn_tt(const Vector2D& pos);
    bool is_in_spawn_ct(const Vector2D& pos);

    void add_tiles_row(std::vector<MapTileType>&& tiles_row);
    void add_bullet(Bullet&& bullet);
    void add_spawn_tt(Vector2D&& spawn_tt);
    void add_spawn_ct(Vector2D&& spawn_ct);
    void add_bomb_site(Vector2D&& bomb_site);
    
    Vector2D random_initial_pos(Team team);

    void update_player_pos(const std::string& player_name, Vector2D&& pos);

    void process_melee_attack(Knife&& knife);

    void place_bomb(Bomb&& bomb);

    void update();

    ~Map();
};

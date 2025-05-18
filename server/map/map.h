#pragma once

#include <string>
#include <vector>
#include <map>

#include "common/models.h"
#include "server/utils/vector_2d.h"
#include "map_cons.h"
#include "server/weapons/bullet.h"
#include "server/weapons/knife.h"
#include "server/weapons/bomb.h"

class Map {
private:
    std::string name;
    std::vector<std::vector<TileType>> tiles;
    std::vector<Vector2D> spawns_tts;
    std::vector<Vector2D> spawns_cts;
    std::vector<Vector2D> bomb_zones;
    std::map<std::string, Vector2D> players;
    std::vector<Bullet> bullets;

public:
    Map();

    bool is_collidable(const Vector2D& pos);

    Vector2D random_initial_pos(Team team);

    void update_player_pos(const std::string& player_name, Vector2D&& pos);

    void add_bullet(Bullet&& bullet);

    void process_melee_attack(Knife&& knife);

    void place_bomb(Bomb&& bomb);

    void update();

    ~Map() {}
};

#pragma once

#include <string>
#include <vector>

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
    std::vector<Bullet> bullets;

public:
    Map() {}

    void add_bullet(Bullet&& bullet) {
        (void)bullet;
    }

    void process_melee_attack(Knife&& knife) {
        (void)knife;
    }

    void place_bomb(Bomb&& bomb) {
        (void)bomb;
    }

    void update() {}

    ~Map() {}
};

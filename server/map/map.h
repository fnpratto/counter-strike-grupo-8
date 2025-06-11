#pragma once

#include <string>
#include <vector>

#include "common/utils/vector_2d.h"

#include "box.h"
#include "floor.h"
#include "wall.h"

struct Map {
    std::string name;
    int max_players;

    std::vector<Floor> floors;
    std::vector<Wall> walls;
    std::vector<Box> boxes;
    std::vector<Vector2D> spawns_tts;
    std::vector<Vector2D> spawns_cts;
    std::vector<Vector2D> bomb_sites;

    Map(const std::string& name, int max_players): name(name), max_players(max_players) {}

    void validate() const {
        if (spawns_tts.empty())
            throw std::runtime_error("Map '" + name + "' has no Terrorist spawns");
        if (spawns_cts.empty())
            throw std::runtime_error("Map '" + name + "' has no Counter-Terrorist spawns");
    }
};

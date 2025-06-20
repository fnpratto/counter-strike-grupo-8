#pragma once

#include <map>
#include <string>
#include <vector>

#include "tile.h"

class Map {
private:
    std::string name;
    int max_players;

    std::map<Vector2D, const Tile> tiles;
    std::vector<const Tile&> spawns_tts;
    std::vector<const Tile&> spawns_cts;
    std::vector<const Tile&> bomb_sites;

public:
    Map(const std::string& name, int max_players);

    std::string get_name() const;
    int get_max_players() const;
    const std::map<Vector2D, const Tile>& get_tiles() const;
    const std::vector<const Tile&>& get_spawns_tts() const;
    const std::vector<const Tile&>& get_spawns_cts() const;
    const std::vector<const Tile&>& get_bomb_sites() const;

    void validate() const;

    void add_tile(Tile&& tile);
};

#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "tile.h"

class Map {
private:
    std::string name;
    int max_players;
    int height;
    int width;

    std::vector<std::vector<std::optional<Tile>>> tiles;
    std::vector<std::reference_wrapper<Tile>> collidables;
    std::vector<std::reference_wrapper<Tile>> spawns_tts;
    std::vector<std::reference_wrapper<Tile>> spawns_cts;
    std::vector<std::reference_wrapper<Tile>> bomb_sites;

public:
    Map(const std::string& name, int max_players, int height, int width);

    std::string get_name() const;
    int get_max_players() const;
    int get_height() const;
    int get_width() const;
    const std::vector<std::vector<std::optional<Tile>>>& get_tiles() const;
    const std::vector<std::reference_wrapper<Tile>>& get_collidables() const;
    const std::vector<std::reference_wrapper<Tile>>& get_spawns_tts() const;
    const std::vector<std::reference_wrapper<Tile>>& get_spawns_cts() const;
    const std::vector<std::reference_wrapper<Tile>>& get_bomb_sites() const;

    void validate() const;

    void add_tile(Tile&& tile);
};

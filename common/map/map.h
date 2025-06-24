#pragma once

#include <functional>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "common/models.h"

#include "tile.h"

class Map {
    std::string name;
    int max_players;
    int height;
    int width;

    std::vector<std::vector<std::optional<Tile>>> tiles;
    std::vector<std::reference_wrapper<Tile>> collidables;
    std::vector<std::reference_wrapper<Tile>> spawns_tts;
    std::vector<std::reference_wrapper<Tile>> spawns_cts;
    std::vector<std::reference_wrapper<Tile>> bomb_sites;
    std::vector<std::pair<GunType, Vector2D>> guns;

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
    const std::vector<std::pair<GunType, Vector2D>>& get_guns() const;

    void validate() const;

    void add_tile(Tile&& tile);
    void add_gun(GunType gun_type, const Vector2D& pos);
};

#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <variant>

#include "common/map/tile.h"
#include "common/utils/vector_2d.h"
#include "server/player/player.h"

using TileRef = std::reference_wrapper<const Tile>;
using PlayerRef = std::reference_wrapper<const std::unique_ptr<Player>>;

class Target: public std::variant<TileRef, PlayerRef> {
private:
    Vector2D hit_pos;

public:
    using std::variant<TileRef, PlayerRef>::variant;

    Target(std::variant<TileRef, PlayerRef> target, Vector2D hit_pos):
            std::variant<TileRef, PlayerRef>(target), hit_pos(hit_pos) {}

    bool is_tile() const { return std::holds_alternative<TileRef>(*this); }
    bool is_player() const { return std::holds_alternative<PlayerRef>(*this); }

    const Tile& get_tile() const { return std::get<TileRef>(*this).get(); }
    const std::unique_ptr<Player>& get_player() const { return std::get<PlayerRef>(*this).get(); }

    Vector2D get_hit_pos() const { return hit_pos; }
};

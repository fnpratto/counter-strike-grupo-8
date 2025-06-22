#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <variant>

#include "common/map/tile.h"
#include "server/player/player.h"

using TileRef = std::reference_wrapper<const Tile>;
using PlayerRef = std::reference_wrapper<const std::unique_ptr<Player>>;

class Target: public std::variant<TileRef, PlayerRef> {
public:
    using std::variant<TileRef, PlayerRef>::variant;

    bool is_tile() const { return std::holds_alternative<TileRef>(*this); }
    bool is_player() const { return std::holds_alternative<PlayerRef>(*this); }

    const Tile& get_tile() const { return std::get<TileRef>(*this).get(); }
    const std::unique_ptr<Player>& get_player() const { return std::get<PlayerRef>(*this).get(); }

    Vector2D get_pos() const {
        if (is_tile())
            return get_tile().pos;
        else if (is_player())
            return get_player()->get_hitbox().center;
        throw std::bad_variant_access();
    }
};

#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <variant>

#include "common/map/box.h"
#include "common/map/wall.h"
#include "server/player/player.h"

using WallRef = std::reference_wrapper<const Wall>;
using BoxRef = std::reference_wrapper<const Box>;
using PlayerRef = std::reference_wrapper<const std::unique_ptr<Player>>;

class Target: public std::variant<WallRef, BoxRef, PlayerRef> {
public:
    using std::variant<WallRef, BoxRef, PlayerRef>::variant;

    bool is_wall() const { return std::holds_alternative<WallRef>(*this); }
    bool is_box() const { return std::holds_alternative<BoxRef>(*this); }
    bool is_player() const { return std::holds_alternative<PlayerRef>(*this); }

    WallRef get_wall() const { return std::get<WallRef>(*this); }
    BoxRef get_box() const { return std::get<BoxRef>(*this); }
    PlayerRef get_player() const { return std::get<PlayerRef>(*this); }

    Vector2D get_pos() const {
        if (is_wall())
            return get_wall().get().get_pos();
        else if (is_box())
            return get_box().get().get_pos();
        else if (is_player())
            return get_player().get()->get_pos();
        throw std::bad_variant_access();
    }
};

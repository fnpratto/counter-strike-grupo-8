#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "common/utils/vector_2d.h"
#include "server/attack_effects/attack_effect.h"
#include "server/map/map.h"
#include "server/player/player.h"

#include "target_type.h"

class PhysicsSystem {
    static constexpr int METER_SIZE = 64;
    static constexpr int HITBOX_RADIUS = METER_SIZE / 2;

private:
    Map map;
    const std::map<std::string, std::unique_ptr<Player>>& players;

    template <typename T>
    std::optional<Target> get_closest_tile(const std::string& origin_p_name, const Vector2D& dir,
                                           const std::vector<T>& vector);
    std::optional<Target> get_closest_player(const std::string& origin_p_name, const Vector2D& dir);

    bool is_in_same_cuadrant(Vector2D target_pos, Vector2D player_pos, Vector2D aim_dir);
    bool player_is_hit(Vector2D target_pos, Vector2D player_pos, Vector2D aim_dir);
    bool tile_is_hit(Vector2D target_pos, Vector2D player_pos, Vector2D aim_dir);

public:
    PhysicsSystem(Map&& map, const std::map<std::string, std::unique_ptr<Player>>& players);

    template <typename T>
    T unit_to_meter(const T& v) const {
        return v * METER_SIZE;
    }
    template <typename T>
    T meter_to_unit(const T& v) const {
        return v / METER_SIZE;
    }

    Vector2D random_spawn_tt_pos() const;
    Vector2D random_spawn_ct_pos() const;
    bool player_in_spawn(const std::string& player_name) const;

    Vector2D calculate_step(const Vector2D& dir) const;

    std::optional<Target> get_closest_target(const std::string& origin_p_name, const Vector2D& dir,
                                             int max_range);
};

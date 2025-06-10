#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "common/map/map.h"
#include "common/utils/vector_2d.h"
#include "server/attack_effects/attack_effect.h"
#include "server/player/player.h"

class PhysicsSystem {
private:
    Map map;
    const std::map<std::string, std::unique_ptr<Player>>& players;

    Vector2D map_to_physics_pos(const Vector2D& pos) const;
    Vector2D physics_to_map_pos(const Vector2D& pos) const;

    bool is_in_same_cuadrant(Vector2D target_pos, Vector2D player_pos, Vector2D aim_dir);
    bool player_is_hit(Vector2D target_pos, Vector2D player_pos, Vector2D aim_dir);
    bool wall_is_hit(Vector2D target_pos, Vector2D player_pos, Vector2D aim_dir);

public:
    PhysicsSystem(Map&& map, const std::map<std::string, std::unique_ptr<Player>>& players);

    Vector2D random_spawn_tt_pos() const;
    Vector2D random_spawn_ct_pos() const;
    bool player_in_spawn(const std::string& player_name) const;

    Vector2D calculate_step(const Vector2D& dir) const;
};

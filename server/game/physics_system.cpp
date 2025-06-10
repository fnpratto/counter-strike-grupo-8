#include "physics_system.h"

#include <algorithm>
#include <functional>
#include <limits>
#include <memory>

#include "game_config.h"

#define TILE_SIZE 32.0      // TODO: Change name (related to meters, so we can use ints)
#define HITBOX_RADIUS 16.0  // TODO: Set this value as a fraction of tile_size

PhysicsSystem::PhysicsSystem(Map&& map,
                             const std::map<std::string, std::unique_ptr<Player>>& players):
        map(std::move(map)), players(players) {}

Vector2D PhysicsSystem::random_spawn_tt_pos() const {
    return map_to_physics_pos(map.random_spawn_tt_pos());
}

Vector2D PhysicsSystem::random_spawn_ct_pos() const {
    return map_to_physics_pos(map.random_spawn_ct_pos());
}

bool PhysicsSystem::player_in_spawn(const std::string& player_name) const {
    const std::unique_ptr<Player>& player = players.at(player_name);
    if (player->is_tt())
        return map.is_spawn_tt_pos(physics_to_map_pos(player->get_pos()));
    return map.is_spawn_ct_pos(physics_to_map_pos(player->get_pos()));
}

Vector2D PhysicsSystem::calculate_step(const Vector2D& dir) const {
    float tick_duration = 1.0f / GameConfig::tickrate;  // TODO use clock
    return dir.normalized() * GameConfig::player_speed * tick_duration;
}

Vector2D PhysicsSystem::map_to_physics_pos(const Vector2D& pos) const { return pos * TILE_SIZE; }

Vector2D PhysicsSystem::physics_to_map_pos(const Vector2D& pos) const { return pos / TILE_SIZE; }

// TODO: get_closest()
//          - Take an origin position (Vector2D) and a direction (Vector2D)
//          - Return std::optional<std::variant<const std::reference_wrapper<Wall>,
//          std::reference_wrapper<std::unique_ptr<Player>>>>

bool PhysicsSystem::is_in_same_cuadrant(Vector2D target_pos, Vector2D player_pos,
                                        Vector2D aim_dir) {
    Vector2D dir_to_target = target_pos - player_pos;
    return dir_to_target.dot(aim_dir) > 0;
}

bool PhysicsSystem::player_is_hit(Vector2D target_pos, Vector2D player_pos, Vector2D aim_dir) {
    Vector2D target_distance = target_pos - player_pos;
    float orthogonal_distance = std::abs(target_distance.cross(aim_dir));
    return orthogonal_distance <= HITBOX_RADIUS;
}

bool PhysicsSystem::wall_is_hit(Vector2D target_pos, Vector2D player_pos, Vector2D aim_dir) {
    // AABB bounds
    float tile_size = TILE_SIZE;
    float minX = target_pos.get_x() - tile_size;
    float maxX = target_pos.get_x() + tile_size;
    float minY = target_pos.get_y() - tile_size;
    float maxY = target_pos.get_y() + tile_size;

    // Ray direction
    float aim_x = aim_dir.get_x();
    float aim_y = aim_dir.get_y();

    // Avoid division by zero
    if (aim_x == 0)
        aim_x = 1e-8;
    if (aim_y == 0)
        aim_y = 1e-8;

    // Calculate intersections with vertical and horizontal slabs
    float t1 = (minX - player_pos.get_x()) / aim_x;
    float t2 = (maxX - player_pos.get_x()) / aim_x;
    float t3 = (minY - player_pos.get_y()) / aim_y;
    float t4 = (maxY - player_pos.get_y()) / aim_y;

    // Find intersection intervals
    float tmin = std::max(std::min(t1, t2), std::min(t3, t4));
    float tmax = std::min(std::max(t1, t2), std::max(t3, t4));

    // If tmax < 0, the square is behind the ray
    if (tmax < 0)
        return false;

    // If tmin > tmax, there is no intersection
    if (tmin > tmax)
        return false;

    return true;
}

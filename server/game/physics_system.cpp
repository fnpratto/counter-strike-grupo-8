#include "physics_system.h"

#include <algorithm>
#include <functional>
#include <limits>
#include <memory>

#include "game_config.h"
#include "physics_system_config.h"

PhysicsSystem::PhysicsSystem(Map&& map,
                             const std::map<std::string, std::unique_ptr<Player>>& players):
        map(std::move(map)), players(players) {}

Vector2D PhysicsSystem::random_spawn_tt_pos() const { return map.random_spawn_tt_pos(); }

Vector2D PhysicsSystem::random_spawn_ct_pos() const { return map.random_spawn_ct_pos(); }

bool PhysicsSystem::player_in_spawn(const std::string& player_name) const {
    const std::unique_ptr<Player>& player = players.at(player_name);
    if (player->is_tt())
        return map.is_spawn_tt_pos(player->get_pos());
    return map.is_spawn_ct_pos(player->get_pos());
}

Vector2D PhysicsSystem::calculate_step(const Vector2D& dir) const {
    float tick_duration = 1.0f / GameConfig::tickrate;  // TODO use clock
    return dir.normalized(PhysicsSystemConfig::meter_size) * GameConfig::player_speed *
           tick_duration;
}

std::optional<Target> PhysicsSystem::get_closest_target(const std::string& origin_p_name,
                                                        const Vector2D& dir, int max_range) {
    auto closest_wall = get_closest_tile<Wall>(origin_p_name, dir, map.get_walls());
    auto closest_box = get_closest_tile<Box>(origin_p_name, dir, map.get_boxes());
    auto closest_player = get_closest_player(origin_p_name, dir);

    std::vector<std::optional<Target>> closests = {closest_wall, closest_box, closest_player};

    float min_distance = std::numeric_limits<float>::max();
    std::optional<Target> closest_target;
    for (const auto& t: closests) {
        if (!t.has_value())
            continue;
        Vector2D origin = players.at(origin_p_name)->get_pos();
        float distance = (t->get_pos() - origin).length();
        if (distance < min_distance) {
            min_distance = distance;
            closest_target = t;
        }
    }

    if (closest_target.has_value() && min_distance > max_range)
        return std::optional<Target>{};

    return closest_target;
}

template <typename T>
std::optional<Target> PhysicsSystem::get_closest_tile(const std::string& origin_p_name,
                                                      const Vector2D& dir,
                                                      const std::vector<T>& vector) {
    std::optional<Target> closest_target;
    Vector2D origin = players.at(origin_p_name)->get_pos();
    float min_distance = std::numeric_limits<float>::max();
    for (const T& t: vector) {
        Vector2D target_pos = t.get_pos();
        if (is_in_same_cuadrant(target_pos, origin, dir) && tile_is_hit(target_pos, origin, dir)) {
            float distance = (target_pos - origin).length();
            if (distance < min_distance) {
                min_distance = distance;
                closest_target = t;
            }
        }
    }
    return closest_target;
}

std::optional<Target> PhysicsSystem::get_closest_player(const std::string& origin_p_name,
                                                        const Vector2D& dir) {
    std::optional<Target> closest_target;
    Vector2D origin = players.at(origin_p_name)->get_pos();
    float min_distance = std::numeric_limits<float>::max();
    for (const auto& [p_name, p]: players) {
        if (origin_p_name == p_name)
            continue;
        Vector2D target_pos = p->get_pos();
        if (is_in_same_cuadrant(target_pos, origin, dir) &&
            player_is_hit(target_pos, origin, dir)) {
            float distance = (target_pos - origin).length();
            if (distance < min_distance) {
                min_distance = distance;
                closest_target = PlayerRef(p);
            }
        }
    }
    return closest_target;
}

bool PhysicsSystem::is_in_same_cuadrant(Vector2D target_pos, Vector2D player_pos,
                                        Vector2D aim_dir) {
    Vector2D dir_to_target = target_pos - player_pos;
    return dir_to_target.dot(aim_dir) > 0;
}

bool PhysicsSystem::player_is_hit(Vector2D target_pos, Vector2D player_pos, Vector2D aim_dir) {
    Vector2D target_distance = target_pos - player_pos;
    float orthogonal_distance = std::abs(target_distance.cross(aim_dir));
    return orthogonal_distance <= PhysicsSystemConfig::player_hitbox_radius;
}

bool PhysicsSystem::tile_is_hit(Vector2D target_pos, Vector2D player_pos, Vector2D aim_dir) {
    // AABB bounds
    float minX = target_pos.get_x();
    float maxX = target_pos.get_x();
    float minY = target_pos.get_y();
    float maxY = target_pos.get_y();

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

#include "physics_system.h"

#include <algorithm>
#include <functional>
#include <limits>
#include <memory>

#include "common/physics/physics_config.h"
#include "server/game/game_config.h"

#include "circular_hitbox.h"
#include "rect_hitbox.h"

PhysicsSystem::PhysicsSystem(Map&& map,
                             const std::map<std::string, std::unique_ptr<Player>>& players,
                             const std::vector<WorldItem<std::unique_ptr<Gun>>>& dropped_guns,
                             const std::optional<WorldItem<Bomb>>& bomb):
        map(std::move(map)), players(players), dropped_guns(dropped_guns), bomb(bomb) {}

Vector2D PhysicsSystem::rand_pos_in_vector(const std::vector<Vector2D>& vector) const {
    int rand_idx = std::rand() % vector.size();
    return vector.at(rand_idx);
}

bool PhysicsSystem::is_pos_in_vector(const std::vector<Vector2D>& vector,
                                     const Vector2D& pos) const {
    return std::any_of(vector.begin(), vector.end(), [&](const Vector2D& p) { return p == pos; });
}

Vector2D PhysicsSystem::random_spawn_tt_pos() const {
    return rand_pos_in_vector(map.spawns_tts) + (PhysicsConfig::meter_size / 2);
}

Vector2D PhysicsSystem::random_spawn_ct_pos() const {
    return rand_pos_in_vector(map.spawns_cts) + (PhysicsConfig::meter_size / 2);
}

bool PhysicsSystem::player_in_spawn(const std::string& player_name) const {
    const std::unique_ptr<Player>& player = players.at(player_name);
    const std::vector<Vector2D>& spawns = player->is_tt() ? map.spawns_tts : map.spawns_cts;
    for (const Vector2D& spawn_pos: spawns) {  // cppcheck-suppress[useStlAlgorithm]
        CircularHitbox player_hitbox = CircularHitbox(player->get_hitbox());
        RectHitbox spawn_hitbox = RectHitbox::tile_hitbox(spawn_pos);
        if (player_hitbox.collides_with_rectangle(spawn_hitbox))
            return true;
    }
    return false;
}

Vector2D PhysicsSystem::calculate_step(const Vector2D& dir) const {
    float tick_duration = 1.0f / GameConfig::tickrate;  // TODO use clock
    return dir.normalized(PhysicsConfig::meter_size) * GameConfig::player_speed * tick_duration;
}

bool PhysicsSystem::is_walkable(const Vector2D& pos) const {
    CircularHitbox player_hitbox = CircularHitbox::player_hitbox(pos);
    for (const Wall& wall: map.walls) {  // cppcheck-suppress[useStlAlgorithm]
        RectHitbox wall_hitbox = RectHitbox::tile_hitbox(wall.get_pos());
        if (player_hitbox.collides_with_rectangle(wall_hitbox))
            return false;
    }
    for (const Box& box: map.boxes) {  // cppcheck-suppress[useStlAlgorithm]
        RectHitbox box_hitbox = RectHitbox::tile_hitbox(box.get_pos());
        if (player_hitbox.collides_with_rectangle(box_hitbox))
            return false;
    }
    return true;
}

std::optional<Target> PhysicsSystem::get_closest_target(const std::string& origin_p_name,
                                                        const Vector2D& dir, int max_range) {
    auto closest_wall = get_closest_tile<Wall>(origin_p_name, dir, map.walls);
    auto closest_box = get_closest_tile<Box>(origin_p_name, dir, map.boxes);
    auto closest_player = get_closest_player(origin_p_name, dir);

    std::vector<std::optional<Target>> closests = {closest_wall, closest_box, closest_player};

    float min_distance = std::numeric_limits<float>::max();
    std::optional<Target> closest_target;
    for (const auto& t: closests) {
        if (!t.has_value())
            continue;
        Vector2D origin = players.at(origin_p_name)->get_hitbox().center;
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
    Vector2D origin = players.at(origin_p_name)->get_hitbox().center;
    float min_distance = std::numeric_limits<float>::max();
    for (const T& target: vector) {
        RectHitbox target_hitbox = RectHitbox::tile_hitbox(target.get_pos());
        if (target_hitbox.is_in_same_cuadrant(origin, dir) && target_hitbox.is_hit(origin, dir)) {
            float distance = (target_hitbox.pos - origin).length();
            if (distance < min_distance) {
                min_distance = distance;
                closest_target = target;
            }
        }
    }
    return closest_target;
}

std::optional<Target> PhysicsSystem::get_closest_player(const std::string& origin_p_name,
                                                        const Vector2D& dir) {
    std::optional<Target> closest_target;
    Vector2D origin = players.at(origin_p_name)->get_hitbox().center;
    float min_distance = std::numeric_limits<float>::max();
    for (const auto& [target_name, target]: players) {  // cppcheck-suppress[unassignedVariable]
        if (origin_p_name == target_name)
            continue;
        CircularHitbox player_hitbox = CircularHitbox(target->get_hitbox());
        if (!target->is_dead() && player_hitbox.is_in_same_cuadrant(origin, dir) &&
            player_hitbox.is_hit(origin, dir)) {
            float distance = (target->get_hitbox().center - origin).length();
            if (distance < min_distance) {
                min_distance = distance;
                closest_target = PlayerRef(target);
            }
        }
    }
    return closest_target;
}

bool PhysicsSystem::player_collides_with_bomb(const std::unique_ptr<Player>& player) const {
    if (!bomb.has_value())
        return false;
    CircularHitbox player_hitbox = CircularHitbox(player->get_hitbox());
    RectHitbox bomb_hitbox = RectHitbox(bomb.value().hitbox);
    return player_hitbox.collides_with_rectangle(bomb_hitbox);
}

std::optional<Vector2D> PhysicsSystem::get_player_colliding_gun_pos(
        const std::unique_ptr<Player>& player) const {
    for (const auto& gun_item: dropped_guns) {
        CircularHitbox player_hitbox = CircularHitbox(player->get_hitbox());
        RectHitbox gun_hitbox = RectHitbox(gun_item.hitbox);
        if (player_hitbox.collides_with_rectangle(gun_hitbox))
            return gun_item.hitbox.pos;
    }
    return std::optional<Vector2D>();
}

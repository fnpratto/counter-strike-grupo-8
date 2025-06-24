#include "physics_system.h"

#include <algorithm>
#include <functional>
#include <limits>
#include <memory>

#include "common/physics/physics_config.h"

#include "circular_hitbox.h"
#include "rect_hitbox.h"

PhysicsSystem::PhysicsSystem(Map&& map,
                             const std::map<std::string, std::unique_ptr<Player>>& players,
                             const std::vector<WorldItem<std::unique_ptr<Gun>>>& dropped_guns,
                             const std::optional<WorldItem<Bomb>>& bomb):
        map(std::move(map)), players(players), dropped_guns(dropped_guns), bomb(bomb) {}

Vector2D PhysicsSystem::translate_to_tile_pos(const Vector2D& pos) const {
    int x = static_cast<int>(pos.get_x() / PhysicsConfig::meter_size);
    int y = static_cast<int>(pos.get_y() / PhysicsConfig::meter_size);
    return Vector2D(x * PhysicsConfig::meter_size, y * PhysicsConfig::meter_size);
}

bool PhysicsSystem::is_pos_out_of_bounds(const Vector2D& pos) const {
    return pos.get_x() < 0 || pos.get_y() < 0 ||
           pos.get_x() >= map.get_width() * PhysicsConfig::meter_size ||
           pos.get_y() >= map.get_height() * PhysicsConfig::meter_size;
}

Vector2D PhysicsSystem::rand_pos_in_vector(
        const std::vector<std::reference_wrapper<Tile>>& vector) const {
    std::vector<std::reference_wrapper<Tile>> filtered_vector;
    std::copy_if(vector.begin(), vector.end(), std::back_inserter(filtered_vector),
                 [](const Tile& tile) { return !tile.is_collidable; });

    if (filtered_vector.empty())
        return Vector2D(0, 0) + (PhysicsConfig::meter_size / 2);

    int rand_idx = rand() % filtered_vector.size();
    Vector2D pos = filtered_vector.at(rand_idx).get().pos + (PhysicsConfig::meter_size / 2);
    while (!can_move_to_pos(pos)) {
        rand_idx = rand() % filtered_vector.size();
        pos = filtered_vector.at(rand_idx).get().pos + (PhysicsConfig::meter_size / 2);
    }

    return pos;
}

Vector2D PhysicsSystem::random_spawn_tt_pos() const {
    return rand_pos_in_vector(map.get_spawns_tts());
}

Vector2D PhysicsSystem::random_spawn_ct_pos() const {
    return rand_pos_in_vector(map.get_spawns_cts());
}

bool PhysicsSystem::player_in_spawn(const std::string& player_name) const {
    const auto& player = players.at(player_name);
    const auto& spawns = player->is_tt() ? map.get_spawns_tts() : map.get_spawns_cts();
    for (const auto& spawn: spawns) {  // cppcheck-suppress[useStlAlgorithm]
        CircularHitbox player_hitbox = CircularHitbox(player->get_hitbox());
        RectHitbox spawn_hitbox = RectHitbox::tile_hitbox(spawn.get().pos);
        if (player_hitbox.collides_with_rectangle(spawn_hitbox))
            return true;
    }
    return false;
}

bool PhysicsSystem::player_in_bomb_site(const std::string& player_name) const {
    const auto& player = players.at(player_name);
    for (const auto& bomb_site: map.get_bomb_sites()) {  // cppcheck-suppress[useStlAlgorithm]
        CircularHitbox player_hitbox = CircularHitbox(player->get_hitbox());
        RectHitbox bomb_site_hitbox = RectHitbox::tile_hitbox(bomb_site.get().pos);
        if (player_hitbox.collides_with_rectangle(bomb_site_hitbox))
            return true;
    }
    return false;
}

Vector2D PhysicsSystem::calculate_new_pos(const std::unique_ptr<Player>& player,
                                          float tick_duration) const {
    Vector2D move_dir = player->get_move_dir();
    if (move_dir == Vector2D(0, 0))
        return player->get_hitbox().center;

    Vector2D step = calculate_step(player->get_speed(), move_dir, tick_duration);
    Vector2D new_pos = player->get_hitbox().center + step;

    if (can_move_to_pos(new_pos))
        return new_pos;

    // Try moving only in X
    Vector2D step_x(step.get_x(), 0);
    Vector2D new_pos_x = player->get_hitbox().center + step_x;
    if (can_move_to_pos(new_pos_x))
        return new_pos_x;

    // Try moving only in Y
    Vector2D step_y(0, step.get_y());
    Vector2D new_pos_y = player->get_hitbox().center + step_y;
    if (can_move_to_pos(new_pos_y))
        return new_pos_y;

    return player->get_hitbox().center;
}

Vector2D PhysicsSystem::calculate_step(int speed, const Vector2D& dir, float tick_duration) const {
    return dir.normalized(PhysicsConfig::meter_size) * speed * tick_duration;
}

bool PhysicsSystem::can_move_to_pos(const Vector2D& pos) const {
    Vector2D translated_pos = translate_to_tile_pos(pos);
    if (is_pos_out_of_bounds(translated_pos))
        return false;

    int collision_check_radius = 2;
    CircularHitbox player_hitbox = CircularHitbox::player_hitbox(pos);
    for (int dx = -collision_check_radius; dx <= collision_check_radius; ++dx) {
        for (int dy = -collision_check_radius; dy <= collision_check_radius; ++dy) {
            Vector2D offset = Vector2D(dx, dy) * PhysicsConfig::meter_size;
            Vector2D translated_pos_with_offset = translated_pos + offset;
            if (is_pos_out_of_bounds(translated_pos_with_offset))
                continue;

            RectHitbox tile_hitbox = RectHitbox::tile_hitbox(translated_pos_with_offset);
            int x = translated_pos_with_offset.get_x() / PhysicsConfig::meter_size;
            int y = translated_pos_with_offset.get_y() / PhysicsConfig::meter_size;
            const auto& tile_opt = map.get_tiles().at(x).at(y);

            if (!tile_opt.has_value() || tile_opt.value().is_collidable) {
                if (player_hitbox.collides_with_rectangle(tile_hitbox))
                    return false;
            }
        }
    }
    return true;
}

std::optional<Target> PhysicsSystem::get_closest_target_in_dir(const std::string& origin_p_name,
                                                               const Vector2D& dir, int max_range) {
    auto closest_collidable_tile = get_closest_collidable_tile(origin_p_name, dir);
    auto closest_player = get_closest_player(origin_p_name, dir);

    Vector2D origin = players.at(origin_p_name)->get_hitbox().center;
    float min_distance = std::numeric_limits<float>::max();
    std::optional<Target> closest_target;

    if (closest_collidable_tile.has_value()) {
        float distance = (closest_collidable_tile->get_hit_pos() - origin).length();
        if (distance < min_distance) {
            min_distance = distance;
            closest_target = closest_collidable_tile;
        }
    }

    if (closest_player.has_value()) {
        float distance = (closest_player->get_hit_pos() - origin).length();
        if (distance < min_distance) {
            min_distance = distance;
            closest_target = closest_player;
        }
    }

    if (closest_target.has_value() && min_distance > max_range)
        return std::optional<Target>{};

    return closest_target;
}

std::optional<Target> PhysicsSystem::get_closest_collidable_tile(const std::string& origin_p_name,
                                                                 const Vector2D& dir) {
    std::optional<Target> closest_collidable_tile;
    Vector2D origin = players.at(origin_p_name)->get_hitbox().center;
    float min_distance = std::numeric_limits<float>::max();
    for (const auto& tile: map.get_collidables()) {
        RectHitbox tile_hitbox = RectHitbox::tile_hitbox(tile.get().pos);
        if (!tile_hitbox.is_in_same_quadrant(origin, dir))
            continue;
        std::optional<Vector2D> hit_pos = tile_hitbox.get_hit_pos(origin, dir);
        if (!hit_pos.has_value())
            continue;
        float distance = (hit_pos.value() - origin).length();
        if (distance < min_distance) {
            min_distance = distance;
            closest_collidable_tile = Target(tile, hit_pos.value());
        }
    }
    return closest_collidable_tile;
}

std::optional<Target> PhysicsSystem::get_closest_player(const std::string& origin_p_name,
                                                        const Vector2D& dir) {
    std::optional<Target> closest_player;
    Vector2D origin = players.at(origin_p_name)->get_hitbox().center;
    float min_distance = std::numeric_limits<float>::max();
    for (const auto& [target_name, target]: players) {  // cppcheck-suppress[unassignedVariable]
        if (origin_p_name == target_name)
            continue;
        CircularHitbox player_hitbox = CircularHitbox(target->get_hitbox());
        if (target->is_dead() || !player_hitbox.is_in_same_quadrant(origin, dir))
            continue;
        std::optional<Vector2D> hit_pos = player_hitbox.get_hit_pos(origin, dir);
        if (!hit_pos.has_value())
            continue;
        float distance = (hit_pos.value() - origin).length();
        if (distance < min_distance) {
            min_distance = distance;
            closest_player = Target(target, hit_pos.value());
        }
    }
    return closest_player;
}

std::vector<PlayerRef> PhysicsSystem::get_players_in_radius(const Vector2D& center,
                                                            int radius) const {
    std::vector<PlayerRef> players_in_radius;
    CircularHitbox radius_hitbox(Circle(center, radius));
    for (const auto& [_, player]: players) {
        if (player->is_dead())
            continue;
        CircularHitbox player_hitbox = CircularHitbox(player->get_hitbox());
        if (radius_hitbox.collides_with_circle(player_hitbox))
            players_in_radius.push_back(PlayerRef(player));
    }
    return players_in_radius;
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
            return gun_item.hitbox.get_pos();
    }
    return std::optional<Vector2D>();
}

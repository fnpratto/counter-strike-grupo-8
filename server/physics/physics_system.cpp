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

Vector2D PhysicsSystem::rand_pos_in_vector(
        const std::vector<std::reference_wrapper<Tile>>& vector) const {
    int rand_idx = std::rand() % vector.size();
    return vector.at(rand_idx).get().pos;
}

Vector2D PhysicsSystem::random_spawn_tt_pos() const {
    return rand_pos_in_vector(map.get_spawns_tts()) + (PhysicsConfig::meter_size / 2);
}

Vector2D PhysicsSystem::random_spawn_ct_pos() const {
    return rand_pos_in_vector(map.get_spawns_cts()) + (PhysicsConfig::meter_size / 2);
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

Vector2D PhysicsSystem::calculate_new_pos(const std::unique_ptr<Player>& player) const {
    Vector2D move_dir = player->get_move_dir();
    if (move_dir == Vector2D(0, 0))
        return player->get_hitbox().center;

    Vector2D step = calculate_step(move_dir);
    Vector2D new_pos = player->get_hitbox().center + step;

    if (!can_move_to_pos(new_pos))
        return player->get_hitbox().center;

    return new_pos;
}

Vector2D PhysicsSystem::calculate_step(const Vector2D& dir) const {
    float tick_duration = 1.0f / GameConfig::tickrate;  // TODO use clock
    return dir.normalized(PhysicsConfig::meter_size) * GameConfig::player_speed * tick_duration;
}

// TODO: Refactor collision detection to avoid iterating over all tiles
bool PhysicsSystem::can_move_to_pos(const Vector2D& pos) const {
    CircularHitbox player_hitbox = CircularHitbox::player_hitbox(pos);
    for (int x = 0; x < map.get_width(); ++x) {
        for (int y = 0; y < map.get_height(); ++y) {
            RectHitbox tile_hitbox =
                    RectHitbox::tile_hitbox(Vector2D(x, y) * PhysicsConfig::meter_size);
            const auto& tile_opt = map.get_tiles().at(x).at(y);

            if (!tile_opt.has_value()) {
                if (player_hitbox.collides_with_rectangle(tile_hitbox))
                    return false;
                continue;
            }

            const Tile& tile = tile_opt.value();
            if (!tile.is_collidable)
                continue;

            if (player_hitbox.collides_with_rectangle(tile_hitbox))
                return false;
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
        float distance = (closest_collidable_tile->get_pos() - origin).length();
        if (distance < min_distance) {
            min_distance = distance;
            closest_target = closest_collidable_tile;
        }
    }

    if (closest_player.has_value()) {
        float distance = (closest_player->get_pos() - origin).length();
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
        if (tile_hitbox.is_in_same_quadrant(origin, dir) && tile_hitbox.is_hit(origin, dir)) {
            float distance = (tile_hitbox.get_pos() - origin).length();
            if (distance < min_distance) {
                min_distance = distance;
                closest_collidable_tile = tile.get();
            }
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
        if (!target->is_dead() && player_hitbox.is_in_same_quadrant(origin, dir) &&
            player_hitbox.is_hit(origin, dir)) {
            float distance = (target->get_hitbox().center - origin).length();
            if (distance < min_distance) {
                min_distance = distance;
                closest_player = PlayerRef(target);
            }
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

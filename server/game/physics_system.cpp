#include "physics_system.h"

#include <memory>

#include "game_config.h"

PhysicsSystem::PhysicsSystem(const Map& map,
                             const std::map<std::string, std::unique_ptr<Player>>& players):
        map(map), players(players) {}

Vector2D PhysicsSystem::random_spawn_tt_pos() const { return map.random_spawn_tt_pos(); }

Vector2D PhysicsSystem::random_spawn_ct_pos() const { return map.random_spawn_ct_pos(); }

Vector2D PhysicsSystem::calculate_step(const Vector2D& dir) const {
    float tick_duration = 1.0f / GameConfig::tickrate;
    return dir.normalized() * GameConfig::player_speed * tick_duration;
}

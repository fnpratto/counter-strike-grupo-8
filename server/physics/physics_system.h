#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "common/game/world_item.h"
#include "common/map/map.h"
#include "common/utils/vector_2d.h"
#include "server/attack_effects/attack_effect.h"
#include "server/player/player.h"

#include "target_type.h"

class PhysicsSystem {
private:
    Map map;
    const std::map<std::string, std::unique_ptr<Player>>& players;
    const std::vector<WorldItem<std::unique_ptr<Gun>>>& dropped_guns;
    const std::optional<WorldItem<Bomb>>& bomb;

    Vector2D rand_pos_in_vector(const std::vector<Vector2D>& vector) const;
    bool is_pos_in_vector(const std::vector<Vector2D>& vector, const Vector2D& pos) const;

    template <typename T>
    std::optional<Target> get_closest_tile(const std::string& origin_p_name, const Vector2D& dir,
                                           const std::vector<T>& vector);
    std::optional<Target> get_closest_player(const std::string& origin_p_name, const Vector2D& dir);

    bool player_is_in_same_cuadrant(const std::unique_ptr<Player>& player, Vector2D origin,
                                    Vector2D dir);
    bool player_is_hit(const std::unique_ptr<Player>& player, Vector2D origin, Vector2D dir);

public:
    PhysicsSystem(Map&& map, const std::map<std::string, std::unique_ptr<Player>>& players,
                  const std::vector<WorldItem<std::unique_ptr<Gun>>>& dropped_guns,
                  const std::optional<WorldItem<Bomb>>& bomb);

    PhysicsSystem(const PhysicsSystem&) = delete;
    PhysicsSystem& operator=(const PhysicsSystem&) = delete;

    PhysicsSystem(PhysicsSystem&&) = delete;
    PhysicsSystem& operator=(PhysicsSystem&&) = delete;

    Vector2D random_spawn_tt_pos() const;
    Vector2D random_spawn_ct_pos() const;
    bool player_in_spawn(const std::string& player_name) const;

    Vector2D calculate_step(const Vector2D& dir) const;
    bool player_can_move_to_pos(const std::unique_ptr<Player>& player, const Vector2D& pos) const;

    std::optional<Target> get_closest_target(const std::string& origin_p_name, const Vector2D& dir,
                                             int max_range);

    bool player_collides_with_bomb(const std::unique_ptr<Player>& player) const;
    std::optional<Vector2D> get_colliding_gun_pos(const std::unique_ptr<Player>& player) const;
};

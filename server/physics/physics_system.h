#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "common/game/world_item.h"
#include "common/map/map.h"
#include "common/utils/vector_2d.h"
#include "server/player/player.h"

#include "target_type.h"

class PhysicsSystem {
    Map map;

    const std::map<std::string, std::unique_ptr<Player>>& players;
    const std::vector<WorldItem<std::unique_ptr<Gun>>>& dropped_guns;
    const std::optional<WorldItem<Bomb>>& bomb;

public:
    PhysicsSystem(Map&& map, const std::map<std::string, std::unique_ptr<Player>>& players,
                  const std::vector<WorldItem<std::unique_ptr<Gun>>>& dropped_guns,
                  const std::optional<WorldItem<Bomb>>& bomb);

    PhysicsSystem(const PhysicsSystem&) = delete;
    PhysicsSystem& operator=(const PhysicsSystem&) = delete;
    PhysicsSystem(PhysicsSystem&&) = delete;
    PhysicsSystem& operator=(PhysicsSystem&&) = delete;

    const Map& get_map() const { return map; }

    // Player spawn and positioning
    Vector2D random_spawn_tt_pos() const;
    Vector2D random_spawn_ct_pos() const;

    // Area checking
    bool player_in_spawn(const std::string& player_name) const;
    bool player_in_bomb_site(const std::string& player_name) const;

    // Movement and positioning
    Vector2D calculate_new_pos(const std::unique_ptr<Player>& player, float tick_duration) const;

    // Target and range finding
    std::optional<Target> get_closest_target_in_dir(const std::string& origin_p_name,
                                                    const Vector2D& dir, int max_range);
    std::vector<PlayerRef> get_players_in_radius(const Vector2D& center, int radius) const;

    // Collision detection
    bool player_collides_with_bomb(const std::unique_ptr<Player>& player) const;
    std::optional<Vector2D> get_player_colliding_gun_pos(
            const std::unique_ptr<Player>& player) const;

private:
    // Vector utilities
    Vector2D rand_pos_in_vector(const std::vector<std::reference_wrapper<Tile>>& vector) const;

    // Movement and positioning
    Vector2D calculate_step(int speed, const Vector2D& dir, float tick_duration) const;
    bool can_move_to_pos(const Vector2D& pos) const;

    // Target finding helpers
    std::optional<Target> get_closest_collidable_tile(const std::string& origin_p_name,
                                                      const Vector2D& dir);
    std::optional<Target> get_closest_player(const std::string& origin_p_name, const Vector2D& dir);

    // Utility methods
    Vector2D translate_to_tile_pos(const Vector2D& pos) const;
    bool is_pos_out_of_bounds(const Vector2D& pos) const;
};

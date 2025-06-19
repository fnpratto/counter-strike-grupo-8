#pragma once

#include <memory>
#include <optional>
#include <stack>
#include <utility>
#include <vector>

#include "common/game/scoreboard_entry.h"
#include "common/models.h"
#include "common/updates/player_update.h"
#include "common/utils/vector_2d.h"
#include "server/clock/clock.h"
#include "server/items/effects/attack_effect.h"
#include "server/logic.h"
#include "server/states/player_state.h"
#include "statuses/player_status.h"

#include "inventory.h"

class Player: public Logic<PlayerState, PlayerUpdate> {
private:
    ScoreboardEntry scoreboard_entry;
    std::unique_ptr<PlayerStatus> status;

public:
    Player(Team team, Circle hitbox);

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    Player(Player&&) = default;
    Player& operator=(Player&&) = default;

    bool is_ready() const;
    bool is_tt() const;
    bool is_ct() const;
    bool is_moving() const;
    bool is_dead() const;
    bool is_attacking();

    Circle get_hitbox() const;
    Vector2D get_move_dir() const;
    Inventory& get_inventory();
    ScoreboardEntry get_scoreboard_entry() const;

    void set_ready();

    void take_damage(int damage);

    void select_team(Team team);

    void select_character(CharacterType character_type);

    void pick_gun(std::unique_ptr<Gun>&& gun);
    void pick_bomb(Bomb&& bomb);

    void start_moving(Vector2D velocity);
    void stop_moving();
    void move_to_pos(Vector2D new_pos);

    void aim(const Vector2D& direction);

    void start_attacking_with_equipped_weapon();
    void stop_attacking();
    std::vector<AttackEffect> attack(TimePoint now);

    void equip_item(ItemSlot slot);

    void reload();

    void add_kill();

    void add_rewards(int score, int bonification);

    std::optional<std::unique_ptr<Gun>> drop_primary_weapon();
    std::optional<Bomb> drop_bomb();

    void start_planting_bomb(TimePoint now);
    void stop_planting_bomb(TimePoint now);
    void start_defusing_bomb(Bomb& bomb, TimePoint now);
    void stop_defusing_bomb(Bomb& bomb, TimePoint now);

    void set_status(std::unique_ptr<PlayerStatus> new_status);
    void handle_start_moving(const Vector2D& velocity);
    void handle_stop_moving();
    void handle_start_attacking();
    void handle_switch_item(ItemSlot slot);
    void handle_start_planting(TimePoint now);
    void handle_stop_planting(TimePoint now);
    void handle_start_defusing(Bomb& bomb, TimePoint now);
    void handle_stop_defusing(Bomb& bomb, TimePoint now);

    void reset();
};

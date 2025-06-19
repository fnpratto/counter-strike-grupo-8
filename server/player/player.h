#pragma once

#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "common/game/scoreboard_entry.h"
#include "common/models.h"
#include "common/updates/player_update.h"
#include "common/utils/vector_2d.h"
#include "server/attack_effects/attack_effect.h"
#include "server/clock/clock.h"
#include "server/logic.h"
#include "server/states/player_state.h"

#include "inventory.h"

class Player: public Logic<PlayerState, PlayerUpdate> {
private:
    ScoreboardEntry scoreboard_entry;

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

    Circle get_hitbox() const;
    Vector2D get_move_dir() const;
    Inventory& get_inventory();
    ScoreboardEntry get_scoreboard_entry() const;

    void set_ready();

    void take_damage(int damage);
    void heal();

    void select_team(Team team);

    void select_character(CharacterType character_type);

    void pick_gun(std::unique_ptr<Gun>&& gun);
    void pick_bomb(Bomb&& bomb);

    void start_moving(Vector2D velocity);
    void stop_moving();
    void move_to_pos(Vector2D new_pos);

    void aim(const Vector2D& direction);

    void start_attacking();
    std::vector<std::unique_ptr<AttackEffect>> attack(TimePoint now);
    void stop_attacking();

    void equip_item(ItemSlot slot);

    void reload();

    void add_kill();

    void add_rewards(int score, int bonification);

    std::optional<std::unique_ptr<Gun>> drop_primary_weapon();
    std::optional<Bomb> drop_bomb();
};

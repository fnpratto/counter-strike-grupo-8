#pragma once

#include "common/models.h"
#include "common/updates/player_update.h"
#include "common/utils/circle.h"
#include "common/utils/vector_2d.h"
#include "server/game/game_config.h"
#include "server/player/inventory.h"

#include "state.h"

class Inventory;

class PlayerState: public State<PlayerUpdate> {
    Team team;
    CharacterType character_type;
    Circle hitbox;
    GameConfig::PlayerConfig player_config;
    Vector2D aim_direction;
    Vector2D velocity;
    int speed;
    bool ready;
    int health;
    ItemSlot equipped_item;
    Inventory inventory;

public:
    PlayerState(Team team, CharacterType character_type, Circle hitbox, Vector2D aim_direction,
                Vector2D velocity, bool ready, ItemSlot equipped_item,
                const GameConfig::PlayerConfig& player_config,
                const GameConfig::ItemsConfig& items_config);

    Team get_team() const;
    CharacterType get_character_type() const;
    Circle get_hitbox() const;
    GameConfig::PlayerConfig get_player_config() const;
    Vector2D get_aim_direction() const;
    Vector2D get_velocity() const;
    int get_speed() const;
    bool get_ready() const;
    int get_health() const;
    ItemSlot get_equipped_item() const;
    Inventory& get_inventory();

    void set_team(Team new_team);
    void set_character_type(CharacterType new_character_type);
    void set_pos(Vector2D new_pos);
    void set_aim_direction(Vector2D new_aim_direction);
    void set_velocity(Vector2D new_velocity);
    void set_ready(bool new_ready);
    void set_health(int new_health);
    void set_equipped_item(ItemSlot new_equipped_item);

    PlayerUpdate get_updates() const override;

    PlayerUpdate get_full_update() const override;  // cppcheck-suppress[virtualCallInConstructor]

    void clear_updates() override;
};

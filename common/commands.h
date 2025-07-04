#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "common/utils/vector_2d.h"

#include "models.h"

/**
 * @class Command
 * @brief Abstract base class for protocol commands.
 */
class Command {
public:
    virtual ~Command() = default;
};

/**
 * @class CreateGameCommand
 * @brief Command to request creation of a new game.
 */
class CreateGameCommand: public Command {
    std::string game_name;
    std::string map_name;
    std::string player_name;

public:
    CreateGameCommand(const std::string& game_name, const std::string& map_name,
                      const std::string& player_name):
            game_name(game_name), map_name(map_name), player_name(player_name) {}

    const std::string& get_game_name() const { return game_name; }
    const std::string& get_map_name() const { return map_name; }
    const std::string& get_player_name() const { return player_name; }
};

/**
 * @class JoinGameCommand
 * @brief Command to request joining an existing game.
 */
class JoinGameCommand: public Command {
    std::string game_name;
    std::string player_name;

public:
    JoinGameCommand(const std::string& game_name, const std::string& player_name):
            game_name(game_name), player_name(player_name) {}

    std::string get_game_name() const { return game_name; }
    std::string get_player_name() const { return player_name; }
};

/**
 * @class ListMapsCommand
 * @brief Command to request a list of available maps.
 */
class ListMapsCommand: public Command {};

/**
 * @class ListGamesCommand
 * @brief Command to request a list of available games.
 */
class ListGamesCommand: public Command {};

/**
 * @class SelectTeamCommand
 * @brief Command to select a team (Terrorist or CounterTerrorist).
 */
class SelectTeamCommand: public Command {
    Team team;

public:
    explicit SelectTeamCommand(Team t): team(t) {}
    Team get_team() const { return team; }
};

/**
 * @class GetCharacterCommand
 * @brief Command to get the character information.
 */
class GetCharactersCommand: public Command {};

/**
 * @class SelectCharacterCommand
 * @brief Command to choose a character for the player.
 */
class SelectCharacterCommand: public Command {
    CharacterType character_type;

public:
    explicit SelectCharacterCommand(CharacterType character_type): character_type(character_type) {}
    CharacterType get_character_type() const { return character_type; }
};

/**
 * @class SetReadyCommand
 * @brief Command to set ready to start game.
 */
class SetReadyCommand: public Command {};

/**
 * @class GetShopPricesCommand
 * @brief Command to request the shop prices.
 */
class GetShopPricesCommand: public Command {};

/**
 * @class BuyGunCommand
 * @brief Command to buy a gun during the preparation phase.
 */
class BuyGunCommand: public Command {
    GunType gun;

public:
    explicit BuyGunCommand(GunType g): gun(g) {}
    GunType get_gun() const { return gun; }
};

/**
 * @class BuyAmmoCommand
 * @brief Command to buy ammo during the preparation phase.
 */
class BuyAmmoCommand: public Command {
    ItemSlot slot;

public:
    explicit BuyAmmoCommand(ItemSlot s): slot(s) {}
    ItemSlot get_slot() const { return slot; }
};

/**
 * @class MoveCommand
 * @brief Command to start moving in a direction.
 */
class MoveCommand: public Command {
    Vector2D direction;

public:
    explicit MoveCommand(Vector2D direction): direction(direction) {}

    Vector2D get_direction() const { return direction; }
};

/**
 * @class StopPlayerCommand
 * @brief Command to stop the player's movement.
 */
class StopPlayerCommand: public Command {
public:
    StopPlayerCommand(): Command() {}
};

/**
 * @class AimCommand
 * @brief Command to update the aiming direction.
 */
class AimCommand: public Command {
    Vector2D direction;

public:
    explicit AimCommand(Vector2D direction): direction(direction) {}

    Vector2D get_direction() const { return direction; }
};

/**
 * @class AttackCommand
 * @brief Command to attack towards the aimed direction.
 */
class AttackCommand: public Command {};

/**
 * @class SwitchItemCommand
 * @brief Command to switch the equipped weapon.
 */
class SwitchItemCommand: public Command {
    ItemSlot slot;

public:
    explicit SwitchItemCommand(ItemSlot s): slot(s) {}
    ItemSlot get_slot() const { return slot; }
};

/**
 * @class ReloadCommand
 * @brief Command to reload the current weapon.
 */
class ReloadCommand: public Command {};

/**
 * @class GetScoreboardCommand
 * @brief Command to request the scoreboard information.
 */
class GetScoreboardCommand: public Command {};

/**
 * @class StartPlantingBombCommand
 * @brief Command to start the bomb planting process.
 */
class StartPlantingBombCommand: public Command {};

/**
 * @class StopPlantingBombCommand
 * @brief Command to stop the bomb planting process.
 */
class StopPlantingBombCommand: public Command {};

/**
 * @class StartDefusingBombCommand
 * @brief Command to start the bomb defusing process.
 */
class StartDefusingBombCommand: public Command {};

/**
 * @class StopDefusingBombCommand
 * @brief Command to stop the bomb defusing process.
 */
class StopDefusingBombCommand: public Command {};

/**
 * @class PickUpItemCommand
 * @brief Command to pick up an item at the current position.
 */
class PickUpItemCommand: public Command {};

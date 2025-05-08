#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

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

public:
    explicit CreateGameCommand(const std::string& name): game_name(name) {}
    std::string get_game_name() const { return game_name; }
};

/**
 * @class JoinGameCommand
 * @brief Command to request joining an existing game.
 */
class JoinGameCommand: public Command {
    std::string game_name;

public:
    explicit JoinGameCommand(const std::string& name): game_name(name) {}
    std::string get_game_name() const { return game_name; }
};

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
 * @class StartGameCommand
 * @brief Command to start the game (sent by the game creator).
 */
class StartGameCommand: public Command {};

/**
 * @class BuyWeaponCommand
 * @brief Command to buy a weapon during the preparation phase.
 */
class BuyWeaponCommand: public Command {
    WeaponType weapon;

public:
    explicit BuyWeaponCommand(WeaponType w): weapon(w) {}
    WeaponType get_weapon() const { return weapon; }
};

/**
 * @class MoveCommand
 * @brief Command to start or stop movement in a direction.
 */
class MoveCommand: public Command {
    MoveDirection direction;
    bool start;

public:
    MoveCommand(MoveDirection dir, bool s): direction(dir), start(s) {}
    MoveDirection get_direction() const { return direction; }
    bool is_start() const { return start; }
};

/**
 * @class AimCommand
 * @brief Command to update the aiming direction.
 */
class AimCommand: public Command {
    float x, y;

public:
    AimCommand(float targetX, float targetY): x(targetX), y(targetY) {}
    float get_x() const { return x; }
    float get_y() const { return y; }
};

/**
 * @class ShootCommand
 * @brief Command to shoot towards the aimed direction.
 */
class ShootCommand: public Command {
    float x, y;

public:
    ShootCommand(float targetX, float targetY): x(targetX), y(targetY) {}
    float get_x() const { return x; }
    float get_y() const { return y; }
};

/**
 * @class SwitchWeaponCommand
 * @brief Command to switch the equipped weapon.
 */
class SwitchWeaponCommand: public Command {
    WeaponSlot slot;

public:
    explicit SwitchWeaponCommand(WeaponSlot s): slot(s) {}
    WeaponSlot get_slot() const { return slot; }
};

/**
 * @class PlantBombCommand
 * @brief Command to plant the bomb at the current position.
 */
class PlantBombCommand: public Command {};

/**
 * @class DefuseBombCommand
 * @brief Command to defuse the bomb.
 */
class DefuseBombCommand: public Command {};

/**
 * @class PickUpItemCommand
 * @brief Command to pick up an item at the current position.
 */
class PickUpItemCommand: public Command {};

/**
 * @class LeaveGameCommand
 * @brief Command to leave the current game.
 */
class LeaveGameCommand: public Command {};

/**
 * @class ReloadCommand
 * @brief Command to reload the current weapon.
 */
class ReloadCommand: public Command {};

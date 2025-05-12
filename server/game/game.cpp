#include "game.h"

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "common/models.h"
#include "server/errors.h"

Game::Game(const std::string& name, const Clock& clock, const GameConfig& config, const Shop& shop): 
        name(name),
        phase(clock, config.get_buying_phase_secs(), config.get_playing_phase_secs()),
        config(config),
        shop(shop),
        num_rounds(0),
        num_terrorists(0),
        num_counter_terrorists(0) {}

RoundPhaseType Game::get_phase_type() const { return phase.get_type(); }

int Game::get_num_players() const { return players.size(); }

int Game::get_num_rounds_played() const { return num_rounds; }

Team Game::get_player_team(const std::string& player_name) const { 
    return players.at(player_name).get_team();
}

Inventory Game::get_player_inventory(const std::string& player_name) const {
    return players.at(player_name).get_inventory();
}

bool Game::is_full() const { return players.size() == config.get_max_players_game(); }

bool Game::team_is_full(Team team) const {
    if (team == Team::Terrorist)
        return num_terrorists == config.get_max_players_team();
    return num_counter_terrorists == config.get_max_players_team();
}

bool Game::is_started() const { return phase.get_type() != RoundPhaseType::NotStarted; }

bool Game::is_invalid_player_name(const std::string& player_name) {
    return players.find(player_name) != players.end() || player_name.empty();
}

void Game::join(const std::string& player_name, Team team) {
    if (is_invalid_player_name(player_name) || is_full() || team_is_full(team) || is_started())
        throw JoinGameError();
    
    Inventory initial_inventory = config.get_default_inventory();
    if (team == Team::Terrorist && num_terrorists == 0)
        initial_inventory.weapons[WeaponSlot::Bomb] = WeaponType::C4;

    players.emplace(player_name, Player(team, initial_inventory));

    if (team == Team::Terrorist) {
        num_terrorists++;
    } else {
        num_counter_terrorists++;
    }
}

void Game::start() {
    if (is_started())
        throw StartGameError();
    phase.start_buying_phase();
}

void Game::player_buy_weapon(const std::string& player_name, WeaponType weapon) {
    int weapon_price = shop.get_weapon_price(weapon);
    int player_money = get_player_inventory(player_name).money;
    if (weapon_price > player_money)
        throw BuyWeaponError();
    players.at(player_name).buy_weapon(weapon, weapon_price);
}

void Game::tick() {
    phase.update();
    if (phase.get_type() == RoundPhaseType::Finished) {
        num_rounds++;
    }
}

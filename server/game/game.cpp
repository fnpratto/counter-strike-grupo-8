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
        shop(shop) {}

void Game::tick(Message msg, const std::string& player_name) {
    handle_msg(msg, player_name);
    phase.update();
    if (phase.get_type() == RoundPhaseType::Finished) {
        num_rounds++;
    }
}

void Game::handle_msg(Message msg, const std::string& player_name) {
    switch (msg.get_type()) {
    case MessageType::JOIN_GAME_CMD:
        join(player_name);
        break;
    case MessageType::SELECT_TEAM_CMD:
        select_player_team(player_name, msg.get_content<SelectTeamCommand>().get_team());
        break;
    case MessageType::START_GAME_CMD:
        start();
        break;
    case MessageType::BUY_WEAPON_CMD:
        player_buy_weapon(player_name, msg.get_content<BuyWeaponCommand>().get_weapon());
        break;
    case MessageType::MOVE_CMD:
        break;
    case MessageType::AIM_CMD:
        break;
    case MessageType::SHOOT_CMD:
        break;
    default:
        break;
    }
}

void Game::join(const std::string& player_name) {
    if (players.find(player_name) != players.end() || player_name.empty() || is_full() || is_started())
        throw JoinGameError();
    
    Team team = choose_player_team();
    players.emplace(player_name, Player(team, config.get_default_inventory()));
    
    if (team == Team::Terrorist) {
        num_terrorists++;
    } else {
        num_counter_terrorists++;
    }
}

Team Game::choose_player_team() {
    if (num_terrorists > num_counter_terrorists)
        return Team::CounterTerrorist;
    return Team::Terrorist;
}

void Game::select_player_team(const std::string& player_name, Team team) {
    if (player_name.empty() || team_is_full(team) || is_started())
        throw SelectTeamError();
    
    Team old_team;
    if (players.at(player_name).is_terrorist()) {
        old_team = Team::Terrorist;
    } else {
        old_team = Team::CounterTerrorist;
    }
    if (old_team == team)
        return;

    players.at(player_name).select_team(team);

    if (team == Team::Terrorist) {
        num_counter_terrorists--;
        num_terrorists++;
    } else {
        num_terrorists--;
        num_counter_terrorists++;
    }
}

void Game::start() {
    if (is_started())
        throw StartGameError();
    
    if (num_terrorists > 0)
        give_bomb_to_terrorists();
    
    phase.start_buying_phase();
}

void Game::give_bomb_to_terrorists() {
    for (auto& [_, player] : players) {
        if (player.is_terrorist()) {
            player.pick_bomb(WeaponType::C4);
            break;
        }
    }
}

void Game::player_buy_weapon(const std::string& player_name, WeaponType weapon) {
    int weapon_price = shop.get_weapon_price(weapon);
    int player_money = get_player_inventory(player_name).money;
    if (weapon_price > player_money)
        throw BuyWeaponError();
    players.at(player_name).buy_weapon(weapon, weapon_price);
}

RoundPhaseType Game::get_phase_type() const { return phase.get_type(); }

int Game::get_num_players() const { return players.size(); }

int Game::get_num_rounds_played() const { return num_rounds; }

Inventory Game::get_player_inventory(const std::string& player_name) const {
    return players.at(player_name).get_inventory();
}

bool Game::is_player_terrorist(const std::string& player_name) const { 
    return players.at(player_name).is_terrorist();
}

bool Game::is_player_counter_terrorist(const std::string& player_name) const { 
    return players.at(player_name).is_counter_terrorist();
}

bool Game::is_full() const { return players.size() == config.get_max_players_game(); }

bool Game::team_is_full(Team team) const {
    if (team == Team::Terrorist)
        return num_terrorists == config.get_max_players_team();
    return num_counter_terrorists == config.get_max_players_team();
}

bool Game::is_started() const { return phase.get_type() != RoundPhaseType::NotStarted; }

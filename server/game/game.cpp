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

#define MAX_NUM_PLAYERS 10

Game::Game(const std::string& name, const Clock& clock): 
        name(name), 
        phase(clock) {}

GamePhaseType Game::get_phase_type() const { return phase.get_type(); }

int Game::get_num_players() const { return players.size(); }

Player& Game::get_player(const std::string& player_name) {
    return players.at(player_name);
}

int Game::get_buying_phase_duration() const { return phase.get_buying_phase_duration(); }

int Game::get_playing_phase_duration() const { return phase.get_playing_phase_duration(); }

int Game::get_max_num_players() const { return MAX_NUM_PLAYERS; }

bool Game::is_full() const { return players.size() == MAX_NUM_PLAYERS; }

bool Game::is_started() const { return phase.get_type() != GamePhaseType::NotStarted; }

bool Game::is_invalid_player_name(const std::string& player_name) {
    return players.find(player_name) != players.end() || player_name.empty();
}

void Game::join(const std::string& player_name) {
    if (is_invalid_player_name(player_name) || is_full() || is_started())
        throw JoinGameError();
    players[player_name] = Player();
}

void Game::start() {
    phase.start();
}

void Game::tick() {
    phase.update();
}

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

#include "errors.h"

void Game::join(const std::string& player_name) {
    if (is_full() || player_name.empty())
        throw JoinGameError();

    // TODO: Implement this method
}

void Game::tick(const Message& msg) {}  // TODO: Implement this method

bool Game::is_full() const { return false; }  // TODO: Implement this method

bool Game::is_finished() const { return false; }  // TODO: Implement this method

void Game::finish() {}  // TODO: Implement this method

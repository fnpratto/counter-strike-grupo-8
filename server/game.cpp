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

bool Game::is_full() const { return false; }  // TODO: Implement this method

bool Game::is_finished() const { return false; }  // TODO: Implement this method

void Game::finish() {}  // TODO: Implement this method

#pragma once

#include <condition_variable>
#include <mutex>
#include <string>

#include "game.h"

class GameMonitor {
    Game game;

    std::mutex mtx;

public:
    explicit GameMonitor(const std::string& name): game(name) {}

    bool is_full() {
        std::lock_guard<std::mutex> lock(mtx);
        return game.is_full();
    }

    bool is_finished() {
        std::lock_guard<std::mutex> lock(mtx);
        return game.is_finished();
    }

    void finish() {
        std::lock_guard<std::mutex> lock(mtx);
        game.finish();
    }
};

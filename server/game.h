#pragma once

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

class Game {
    std::string name;

public:
    explicit Game(const std::string& name): name(name) {}

    bool is_full() const;
    bool is_finished() const;

    void finish();
};

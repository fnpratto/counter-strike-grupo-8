#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "common/message.h"
#include "common/models.h"

#include "errors.h"

class Game {
    std::string name;

public:
    explicit Game(const std::string& name): name(name) {}

    void tick(const Message& msg);

    void join(const std::string& player_name);
    void finish();

    bool is_full() const;
    bool is_finished() const;
};

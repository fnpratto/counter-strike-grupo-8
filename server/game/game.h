#pragma once

#include <atomic>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <utility>

#include "common/message.h"
#include "common/models.h"
#include "server/errors.h"

#include "game_phase.h"
#include "player.h"

class Game {
private:
    std::string name;
    GamePhase phase;
    std::map<std::string, Player> players;

    bool is_invalid_player_name(const std::string& player_name);

public:
    Game(const std::string& name, const Clock& clock);

    GamePhaseType get_phase_type() const;
    int get_num_players() const;
    Player& get_player(const std::string& player_name);
    int get_buying_phase_duration() const;
    int get_playing_phase_duration() const;
    int get_max_num_players() const;

    bool is_full() const;
    bool is_started() const;

    void join(const std::string& player_name);

    void start();

    void tick(const Message& msg);
};

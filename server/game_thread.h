#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "common/message.h"
#include "common/queue.h"
#include "common/responses.h"
#include "common/thread.h"
#include "game/game.h"

#include "pipe.h"

class GameThread: public Thread {
    static constexpr int MSG_BATCH_SIZE = 10;  // Number of messages to process in one tick

    Game game;
    std::mutex mtx;

    std::shared_ptr<Queue<PlayerMessage>> input_queue;  // Shared queue for incoming messages
    std::map<std::string, std::shared_ptr<Queue<Message>>> output_queues;  // Shared output queues

public:
    explicit GameThread(const std::string& name, Map&& map);

    void run() override;

    pipe_t join_game(const std::string& player_name);

    GameInfo get_game_info();

    void stop() override;
};

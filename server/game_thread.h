#pragma once

#include <memory>
#include <string>
#include <vector>

#include "common/message.h"
#include "common/queue.h"
#include "common/thread.h"
#include "game/game.h"

#include "pipe.h"

class GameThread: public Thread {
    static constexpr int MSG_BATCH_SIZE = 10;  // Number of messages to process in one tick

    Game game;
    std::shared_ptr<Queue<Message>> input_queue;  // Shared queue for incoming messages
    std::vector<std::shared_ptr<Queue<Message>>> output_queues;  // Shared output queues

public:
    explicit GameThread(const std::string& name);

    void run() override;

    pipe_t join_game(const std::string& player_name);

    // Check if the game is full
    bool is_full();

    void stop() override;
};

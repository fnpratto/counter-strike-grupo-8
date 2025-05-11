#include "game_thread.h"

GameThread::GameThread(const std::string& name):
        game(name), input_queue(std::make_shared<Queue<Message>>()) {}

void GameThread::run() {
    // Empty implementation for now
}

pipe_t GameThread::join_game(const std::string& player_name) {
    game.join(player_name);

    auto output_queue = std::make_shared<Queue<Message>>();
    output_queues.push_back(output_queue);

    return {input_queue, output_queue};
}

bool GameThread::is_full() { return game.is_full(); }

void GameThread::stop() {
    // Implementation of stop method
    // You might want to add signal mechanism to stop the thread
}

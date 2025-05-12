#include "game_thread.h"

#include "game/clock/real_clock.h"

GameThread::GameThread(const std::string& name):
        game(name, RealClock()), input_queue(std::make_shared<Queue<Message>>()) {}

// TODO: Tick rate
void GameThread::run() {
    Message msg;
    while (should_keep_running()) {
        input_queue->try_pop(msg);
        game.tick(msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

pipe_t GameThread::join_game(const std::string& player_name) {
    game.join(player_name);

    auto output_queue = std::make_shared<Queue<Message>>();
    output_queues.push_back(output_queue);

    return {input_queue, output_queue};
}

bool GameThread::is_full() { return game.is_full(); }

void GameThread::stop() {}  // TODO: finish game

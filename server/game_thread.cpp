#include "game_thread.h"

#include "clock/real_clock.h"
#include "map/map.h"

GameThread::GameThread(const std::string& name):
        game(name, std::make_unique<RealClock>(), Map("map_name", 16)),
        input_queue(std::make_shared<Queue<Message>>()) {}

// TODO: Tick rate
void GameThread::run() {
    while (should_keep_running()) {
        std::vector<Message> msgs;
        for (int i = 0; i < MSG_BATCH_SIZE; ++i) {
            Message msg;
            if (!input_queue->try_pop(msg)) {
                break;  // No more messages to process
            }
            msgs.push_back(msg);
        }
        game.tick(msgs, "player_name");  // TODO Replace "player_name" with actual player name
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

pipe_t GameThread::join_game(const std::string& player_name) {
    GameUpdate initial_state = game.join_player(player_name);

    auto output_queue = std::make_shared<Queue<Message>>();
    output_queues.push_back(output_queue);

    output_queue->push(Message(initial_state));
    return {input_queue, output_queue};
}

bool GameThread::is_full() { return game.is_full(); }

void GameThread::stop() { Thread::stop(); }  // TODO: finish game

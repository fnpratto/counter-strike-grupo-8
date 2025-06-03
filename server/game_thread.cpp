#include "game_thread.h"

#include <utility>

#include "clock/real_clock.h"
#include "map/map.h"
#include "map/map_builder.h"
#include "server/player_message.h"

GameThread::GameThread(const std::string& name):
        game(name, std::make_unique<RealClock>(),
             std::move(MapBuilder("./tests/server/map/map.yaml").build())),
        input_queue(std::make_shared<Queue<PlayerMessage>>()) {}

// TODO: Tick rate
void GameThread::run() {
    while (should_keep_running()) {
        std::vector<PlayerMessage> msgs;
        for (int i = 0; i < MSG_BATCH_SIZE; ++i) {
            PlayerMessage msg;
            if (!input_queue->try_pop(msg))
                break;  // No more messages to process

            msgs.push_back(msg);
        }

        GameUpdate update = game.tick(msgs);
        if (update.has_change()) {
            for (const auto& output_queue: output_queues) {
                output_queue->push(Message(update));
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

// FIXME: Race condition
pipe_t GameThread::join_game(const std::string& player_name) {
    GameUpdate initial_state = game.join_player(player_name);

    auto output_queue = std::make_shared<Queue<Message>>();
    output_queues.push_back(output_queue);

    // Awful hack to ensure that the other players receive the joined player's state
    for (const auto& q: output_queues) {
        q->push(Message(initial_state));
    }

    return {input_queue, output_queue};
}

bool GameThread::is_full() { return game.is_full(); }

void GameThread::stop() { Thread::stop(); }  // TODO: finish game

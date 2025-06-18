#include "game_thread.h"

#include <utility>

#include "clock/real_clock.h"
#include "common/map/map.h"
#include "common/utils/rate_controller.h"
#include "map/map_builder.h"
#include "server/game/game_config.h"
#include "server/player_message.h"

GameThread::GameThread(const std::string& name):
        game(name, std::make_unique<RealClock>(),
             std::move(MapBuilder("./tests/server/map/map.yaml").build())),
        input_queue(std::make_shared<Queue<PlayerMessage>>()) {}

void GameThread::run() {
    RateController rate_controller(GameConfig::tickrate);
    rate_controller.run_at_rate([this]() {
        std::vector<PlayerMessage> msgs;
        for (int i = 0; i < MSG_BATCH_SIZE; ++i) {
            PlayerMessage msg;
            if (!input_queue->try_pop(msg))
                break;  // No more messages to process
            msgs.push_back(msg);
        }

        std::vector<PlayerMessage> out_msgs;
        {
            std::lock_guard<std::mutex> lock(mtx);
            out_msgs = game.tick(msgs);
        }

        for (const auto& out_msg: out_msgs) {
            auto q = output_queues.at(out_msg.get_player_name());
            q->push(out_msg.get_message());
        }

        return should_keep_running();
    });
}

pipe_t GameThread::join_game(const std::string& player_name) {
    std::lock_guard<std::mutex> lock(mtx);
    game.join_player(player_name);

    output_queues[player_name] = std::make_shared<Queue<Message>>();

    return {input_queue, output_queues[player_name]};
}

GameInfo GameThread::get_game_info() {
    std::lock_guard<std::mutex> lock(mtx);
    return GameInfo(game.get_name(), game.get_player_count(), game.get_phase());
}

void GameThread::stop() { Thread::stop(); }  // TODO: finish game

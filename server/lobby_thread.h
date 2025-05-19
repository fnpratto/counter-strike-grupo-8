#pragma once

#include <functional>
#include <memory>
#include <utility>

#include "common/commands.h"
#include "common/thread.h"

#include "lobby_monitor.h"
#include "protocol.h"
#include "receiver.h"
#include "sender.h"

class LobbyThread: public Thread {
private:
    ServerProtocol& protocol;
    LobbyMonitor& lobby_monitor;  // TODO: check lifetime of this reference

    std::function<void(pipe_t)> join_callback;

public:
    LobbyThread(ServerProtocol& proto, LobbyMonitor& lobby_monitor,
                std::function<void(pipe_t)> join_callback);

    void run() override;

    void handle_create_game_cmd(const CreateGameCommand& cmd);
    void handle_join_game_cmd(const JoinGameCommand& cmd);
    void handle_list_games_cmd();
};

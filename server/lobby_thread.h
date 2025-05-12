#pragma once

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
    LobbyMonitor& lobby_monitor;  // TODO: check lifetime of this reference
    std::shared_ptr<ServerProtocol> protocol;

    std::unique_ptr<Sender>& sender;
    std::unique_ptr<Receiver>& receiver;

public:
    explicit LobbyThread(LobbyMonitor& lobby_monitor, std::shared_ptr<ServerProtocol> proto,
                         std::unique_ptr<Sender>& sender, std::unique_ptr<Receiver>& receiver);

    void run() override;

    void handle_create_game_cmd(const CreateGameCommand& cmd);
    void handle_join_game_cmd(const JoinGameCommand& cmd);
    void handle_list_games_cmd();
};

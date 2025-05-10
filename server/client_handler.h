#pragma once

#include <memory>
#include <string>
#include <utility>

#include "common/models.h"
#include "common/socket.h"
#include "common/thread.h"

#include "game.h"
#include "lobby_monitor.h"
#include "protocol.h"

class ClientHandler: public Thread {
private:
    LobbyMonitor& lobby_monitor;
    std::shared_ptr<GameMonitor> game;
    ServerProtocol protocol;

    void handle_create_game(const CreateGameCommand& command);
    void handle_join_game(const JoinGameCommand& command);
    void handle_list_games();

public:
    ClientHandler(Socket&& client_socket, LobbyMonitor& lobby_monitor):
            lobby_monitor(lobby_monitor),
            protocol(std::make_shared<Socket>(std::move(client_socket))) {}

    void run() override;
    void stop() override;
};

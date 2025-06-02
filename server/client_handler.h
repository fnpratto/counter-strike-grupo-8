#pragma once

#include <memory>
#include <string>
#include <utility>

#include "common/models.h"
#include "common/socket.h"

#include "game_thread.h"
#include "lobby_monitor.h"
#include "lobby_thread.h"
#include "protocol.h"
#include "receiver.h"
#include "sender.h"

class ClientHandler {
private:
    ServerProtocol protocol;

    /*
    TODO: lobby_thread lives forever (until the ClientHandler is destroyed)
    we should probably pass a callback to the LobbyThread
    to set the sender and receiver when the game is created/joined
    and then destroy the LobbyThread
    */
    std::unique_ptr<LobbyThread> lobby_thread;
    std::unique_ptr<Sender> sender;
    std::unique_ptr<Receiver> receiver;


public:
    ClientHandler(Socket&& client_socket, LobbyMonitor& lobby_monitor);

    void connect(const std::string& player_name, pipe_t pipe);
    void disconnect();
};

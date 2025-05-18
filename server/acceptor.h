#pragma once

#include <memory>
#include <string>
#include <vector>

#include "common/socket.h"
#include "common/thread.h"

#include "client_handler.h"
#include "lobby_monitor.h"

class Acceptor: public Thread {
private:
    Socket socket;
    LobbyMonitor lobby_monitor;

    // @note These are pointers to allow for easy movement
    std::vector<std::unique_ptr<ClientHandler>> clients;

public:
    explicit Acceptor(const std::string& port): socket(Socket(port.c_str())), lobby_monitor() {}

    void run() override;
    void stop() override;

    void reap();
};

#include "client_handler.h"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "errors.h"

ClientHandler::ClientHandler(Socket&& client_socket, LobbyMonitor& lobby_monitor):
        protocol(std::make_shared<ServerProtocol>(std::move(client_socket))),
        lobby_thread(std::make_unique<LobbyThread>(lobby_monitor, protocol, sender, receiver)) {
    lobby_thread->start();
}

void ClientHandler::disconnect() {
    if (lobby_thread) {
        lobby_thread->stop();
        lobby_thread->join();
    }
    if (sender) {
        sender->stop();
        sender->join();
    }
    if (receiver) {
        receiver->stop();
        receiver->join();
    }
    protocol->close();
}

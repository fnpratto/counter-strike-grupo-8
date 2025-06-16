#include "client_handler.h"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "server/player_message.h"

#include "errors.h"

ClientHandler::ClientHandler(Socket&& client_socket, LobbyMonitor& lobby_monitor):
        protocol(ServerProtocol(std::make_shared<Socket>(std::move(client_socket)))),
        lobby_thread(std::make_unique<LobbyThread>(
                protocol, lobby_monitor, [this](const std::string& player_name, pipe_t pipe) {
                    this->connect(player_name, std::move(pipe));
                })) {
    lobby_thread->start();
}

void ClientHandler::connect(const std::string& player_name, pipe_t pipe) {
    receiver = std::make_unique<Receiver>(player_name, protocol, std::move(pipe.first));
    sender = std::make_unique<Sender>(protocol, std::move(pipe.second));

    sender->start();
    receiver->start();
}

void ClientHandler::disconnect() {
    protocol.close();
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
}

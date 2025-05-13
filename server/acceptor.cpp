#include "acceptor.h"

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <sys/socket.h>

#include "common/liberror.h"

#include "client_handler.h"

void Acceptor::run() {
    while (should_keep_running()) {
        try {
            Socket peer = socket.accept();

            auto client_handler = std::make_unique<ClientHandler>(std::move(peer), lobby_monitor);
            clients.push_back(std::move(client_handler));

            reap();
        } catch (const LibError& e) {
            // socket shutdown
            break;
        } catch (const std::exception& e) {
            std::cerr << "Error accepting client: " << e.what() << std::endl;
            break;
        }
    }
}

void Acceptor::reap() {
    // TODO: correct
    // auto it = std::remove_if(clients.begin(), clients.end(), [](const auto& client) {
    //     if (!client->is_alive()) {
    //         client->join();
    //         return true;
    //     }
    //     return false;
    // });
    // clients.erase(it, clients.end());

    lobby_monitor.reap();
}

void Acceptor::stop() {
    Thread::stop();

    for (auto& client: clients) client->disconnect();

    // Shutdown and close the socket in case we're stuck on accept()
    socket.shutdown(SHUT_RDWR);
    socket.close();
}

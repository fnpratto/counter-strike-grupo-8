#include "sender.h"

#include <iostream>
#include <thread>

void ClientSender::run() {
    try {
        while (should_keep_running()) {
            Message message = queue.pop();

            if (message.get_type() != MessageType::NONE)
                protocol->send(message);
        }
    } catch (const std::exception& e) {
        if (!should_keep_running())
            return;

        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void ClientSender::stop() {
    Thread::stop();
    queue.close();
    protocol->close();
}

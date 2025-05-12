#include "sender.h"

#include <iostream>
#include <thread>

void ClientSender::run() {
    try {
        Message message;
        while (should_keep_running()) {
            queue.try_pop(message);

            if (message.get_type() != MessageType::NONE)
                protocol->send(message);

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    } catch (const std::exception& e) {
        if (!should_keep_running())
            return;

        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void ClientSender::stop() {
    Thread::stop();
    protocol->close();
}

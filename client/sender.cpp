#include "sender.h"

#include <iostream>
#include <thread>

void ClientSender::run() {
    try {
        while (should_keep_running()) {
            Message message = Message();

            if (queue.try_pop(message) && message.get_type() != MessageType::NONE)
                protocol.send(message);

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

    if (protocol.is_open())
        protocol.close();
}

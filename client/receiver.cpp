#include "receiver.h"

#include <iostream>
#include <thread>

#include "common/errors.h"

void ClientReceiver::run() {
    try {
        while (should_keep_running()) {
            Message message = protocol->recv();

            if (message.get_type() != MessageType::NONE)
                queue.push(message);
        }
    } catch (const ServerDisconnectError& e) {
        stop();
        return;
    } catch (const std::exception& e) {
        if (!should_keep_running())
            return;

        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void ClientReceiver::stop() {
    Thread::stop();
    protocol->close();
}

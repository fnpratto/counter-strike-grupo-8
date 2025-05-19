#include "receiver.h"

#include <iostream>
#include <thread>

#include "errors.h"

void ClientReceiver::run() {
    try {
        while (should_keep_running()) {
            Message message = protocol->recv();

            if (message.get_type() != MessageType::NONE)
                queue.push(message);

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
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

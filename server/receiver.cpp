#include "receiver.h"

#include <iostream>
#include <utility>

Receiver::Receiver(ServerProtocol& protocol, std::shared_ptr<Queue<Message>> queue):
        protocol(protocol), queue(std::move(queue)) {}

void Receiver::run() {
    while (should_keep_running()) {
        try {
            auto msg = protocol.recv();
            queue->push(msg);
        } catch (const ClosedQueue&) {
            stop();
            break;
        } catch (const std::exception& e) {
            if (protocol.is_closed()) {
                stop();
                break;
            }

            std::cerr << "Receiver error: " << e.what() << std::endl;
        }
    }
}

void Receiver::stop() {
    Thread::stop();
    protocol.close();
}

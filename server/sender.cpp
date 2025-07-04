#include "sender.h"

#include <iostream>

Sender::Sender(ServerProtocol& protocol, std::shared_ptr<Queue<Message>>&& queue):
        protocol(protocol), queue(std::move(queue)) {}

void Sender::run() {
    while (should_keep_running()) {
        try {
            Message message = queue->pop();
            protocol.send(message);
        } catch (const ClosedQueue&) {
            break;
        } catch (const std::exception& e) {
            std::cerr << "Sender error: " << e.what() << std::endl;
        }
    }
}

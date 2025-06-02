#include "receiver.h"

#include <iostream>
#include <string>
#include <utility>

Receiver::Receiver(const std::string& player_name, ServerProtocol& protocol,
                   std::shared_ptr<Queue<PlayerMessage>> queue):
        player_name(player_name), protocol(protocol), queue(std::move(queue)) {}

void Receiver::run() {
    while (should_keep_running()) {
        try {
            auto msg = protocol.recv();
            queue->push(PlayerMessage(player_name, msg));
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

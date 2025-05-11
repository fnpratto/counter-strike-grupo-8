#pragma once

#include <memory>
#include <utility>

#include "common/queue.h"
#include "common/thread.h"

#include "protocol.h"

class Sender: public Thread {
    std::shared_ptr<ServerProtocol> protocol;

    std::shared_ptr<Queue<Message>> queue;

public:
    explicit Sender(std::shared_ptr<ServerProtocol> protocol,
                    std::shared_ptr<Queue<Message>>&& queue):
            protocol(std::move(protocol)), queue(std::move(queue)) {}

    void run() override {
        while (should_keep_running()) {
            try {
                Message message = queue->pop();
                protocol->send(message);
            } catch (const ClosedQueue&) {
                break;
            } catch (const std::exception& e) {
                std::cerr << "Sender error: " << e.what() << std::endl;
            }
        }
    }

    void stop() override {
        Thread::stop();
        protocol->close();
    }
};

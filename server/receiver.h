#pragma once

#include <memory>
#include <utility>

#include "common/queue.h"
#include "common/thread.h"

#include "protocol.h"

class Receiver: public Thread {
    std::shared_ptr<ServerProtocol> protocol;

    std::shared_ptr<Queue<Message>> queue;

public:
    explicit Receiver(std::shared_ptr<ServerProtocol> protocol,
                      std::shared_ptr<Queue<Message>> queue):
            protocol(std::move(protocol)), queue(std::move(queue)) {}

    void run() override {
        while (should_keep_running()) {
            try {
                auto msg = protocol->recv();
                queue->push(msg);
            } catch (const ClosedQueue&) {
                break;
            } catch (const std::exception& e) {
                std::cerr << "Receiver error: " << e.what() << std::endl;
            }
        }
    }

    void stop() override {
        Thread::stop();
        protocol->close();
    }
};

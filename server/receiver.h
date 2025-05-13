#pragma once

#include <memory>

#include "common/queue.h"
#include "common/thread.h"

#include "protocol.h"

class Receiver: public Thread {
private:
    ServerProtocol& protocol;
    std::shared_ptr<Queue<Message>> queue;

public:
    explicit Receiver(ServerProtocol& protocol, std::shared_ptr<Queue<Message>> queue);

    void run() override;
    void stop() override;
};

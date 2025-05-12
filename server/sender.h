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
                    std::shared_ptr<Queue<Message>>&& queue);

    void run() override;
};

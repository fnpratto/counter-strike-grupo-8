#pragma once

#include <memory>
#include <string>

#include "common/queue.h"
#include "common/thread.h"
#include "server/player_message.h"

#include "protocol.h"

class Receiver: public Thread {
private:
    std::string player_name;
    ServerProtocol& protocol;
    std::shared_ptr<Queue<PlayerMessage>> queue;

public:
    explicit Receiver(const std::string& player_name, ServerProtocol& protocol,
                      std::shared_ptr<Queue<PlayerMessage>> queue);

    void run() override;
};

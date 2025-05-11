#pragma once
#include <memory>
#include <utility>

#include "common/message.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"

#include "protocol.h"

/**
 * @class ClientReceiver
 * @brief Thread that receives messages from server.
 *
 * Listens on ClientProtocol and pushes incoming Message objects into a queue.
 */
class ClientReceiver: public Thread {
private:
    Queue<Message>& queue;
    std::shared_ptr<ClientProtocol> protocol;

public:
    /**
     * @brief Constructs a ClientReceiver.
     * @param skt Shared pointer to a connected Socket.
     * @param queue Reference to the message queue to populate.
     */
    ClientReceiver(std::shared_ptr<ClientProtocol> p, Queue<Message>& queue):
            queue(queue), protocol(std::move(p)) {}

    void run() override;
    void stop() override;
};

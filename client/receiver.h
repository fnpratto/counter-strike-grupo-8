#pragma once
#include <memory>
#include <utility>

#include "common/message.h"

#include "protocol.h"
#include "queue.h"
#include "socket.h"
#include "thread.h"

/**
 * @class ClientReceiver
 * @brief Thread that receives messages from server.
 *
 * Listens on ClientProtocol and pushes incoming Message objects into a queue.
 */
class ClientReceiver: public Thread {
private:
    Queue<Message>& queue;
    ClientProtocol protocol;

public:
    /**
     * @brief Constructs a ClientReceiver.
     * @param skt Shared pointer to a connected Socket.
     * @param queue Reference to the message queue to populate.
     */
    ClientReceiver(std::shared_ptr<Socket> skt, Queue<Message>& queue):
            queue(queue), protocol(std::move(skt)) {}

    void run() override;
    void stop() override;
};

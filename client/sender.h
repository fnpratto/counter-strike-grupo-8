#pragma once
#include <memory>
#include <utility>

#include "common/message.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"

#include "protocol.h"

/**
 * @class ClientSender
 * @brief Thread that sends queued messages from client to server.
 *
 * Consumes Message objects from an input queue and sends them via ClientProtocol.
 */
class ClientSender: public Thread {
private:
    Queue<Message>& queue;
    ClientProtocol protocol;

public:
    ClientSender(std::shared_ptr<Socket> skt, Queue<Message>& queue):
            queue(queue), protocol(std::move(skt)) {}

    void run() override;
    void stop() override;
};

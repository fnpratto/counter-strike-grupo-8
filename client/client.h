#pragma once

#include <atomic>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <arpa/inet.h>

#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"

#include "display.h"
#include "input.h"
#include "receiver.h"
#include "sender.h"

class Client {
private:
    std::shared_ptr<Socket> client_socket;

    Queue<Message> input_queue;
    Queue<Message> display_queue;

    std::unique_ptr<Input> input;
    std::unique_ptr<Display> display;

    ClientSender sender;
    ClientReceiver receiver;

public:
    Client(const char* hostname, const char* port);
    void run();
};

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
#include "receiver.h"
#include "sender.h"

class Client {
private:
    std::shared_ptr<ClientProtocol> protocol;

    Queue<Message> pregame_queue;
    Queue<Message> ingame_queue;
    Queue<Message> display_queue;

    std::unique_ptr<Display> display;

    std::unique_ptr<ClientSender> sender;
    std::unique_ptr<ClientReceiver> receiver;

    // New method declarations with snake_case naming
    bool connect_to_server() noexcept;
    void setup_communication();
    void wait_for_game_start();
    void switch_display();
    void cleanup();

public:
    Client();
    void run();
};

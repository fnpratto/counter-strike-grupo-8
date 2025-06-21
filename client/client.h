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
#include "lobby_display.h"
#include "receiver.h"
#include "sender.h"

class Client {
private:
    std::shared_ptr<ClientProtocol> protocol;

    Queue<Message> lobby_input_queue;   // messages input into the lobby display
    Queue<Message> lobby_output_queue;  // messages output by the lobby display
    Queue<Message> game_input_queue;    // messages input into the game
    Queue<Message> game_output_queue;   // messages output by the game

    std::unique_ptr<LobbyDisplay> lobby_display;
    std::unique_ptr<Display> display;

    // pops messages from `game_input_queue` and sends them to the server
    std::unique_ptr<ClientSender> sender;
    // receives messages from the server and pushes them into `game_output_queue`
    std::unique_ptr<ClientReceiver> receiver;

    void switch_display(const std::string& player_name);
    void cleanup();

public:
    Client();
    void run();
};

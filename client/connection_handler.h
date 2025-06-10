#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "common/message.h"
#include "common/queue.h"

#include "display.h"
#include "protocol.h"
#include "receiver.h"
#include "sender.h"

class ConnectionHandler: public Thread {
    Queue<Message>& pregame_queue;
    Queue<Message>& ingame_queue;
    Queue<Message>& display_queue;

    std::shared_ptr<ClientProtocol> protocol;

    std::unique_ptr<ClientSender> sender;
    std::unique_ptr<ClientReceiver> receiver;

    std::string player_name = "";

public:
    ConnectionHandler(Queue<Message>& pregame_queue, Queue<Message>& ingame_queue,
                      Queue<Message>& display_queue);

    void run() override;
    void stop() override;

    std::unique_ptr<ClientSender> get_sender() { return std::move(sender); }
    std::unique_ptr<ClientReceiver> get_receiver() { return std::move(receiver); }
    std::string get_player_name() const { return player_name; }

private:
    bool connect_to_server();
    void setup_communication();
    void wait_for_game_start();
};

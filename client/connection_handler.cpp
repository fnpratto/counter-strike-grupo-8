#include "connection_handler.h"

#include <iostream>
#include <memory>
#include <string>

#include "common/message.h"

#include "protocol.h"
#include "receiver.h"
#include "sender.h"

ConnectionHandler::ConnectionHandler(Queue<Message>& pregame_queue, Queue<Message>& ingame_queue,
                                     Queue<Message>& display_queue):
        pregame_queue(pregame_queue),
        ingame_queue(ingame_queue),
        display_queue(display_queue),
        sender(nullptr),
        receiver(nullptr) {}


void ConnectionHandler::run() {
    if (!connect_to_server())
        return;

    std::cout << "Connection established" << std::endl;

    setup_communication();
    wait_for_game_start();
}

bool ConnectionHandler::connect_to_server() {
    Message msg;
    while (should_keep_running()) {
        try {
            msg = pregame_queue.pop();
        } catch (const ClosedQueue&) {
            std::cerr << "ConnectionHandler: Input queue closed, stopping." << std::endl;
            return false;
        }

        if (msg.get_type() != MessageType::CONN_REQ)
            continue;

        auto conn_req = msg.get_content<ConnectionRequest>();
        try {
            protocol = std::make_shared<ClientProtocol>(
                    Socket(conn_req.get_ip().c_str(), conn_req.get_port().c_str()));
        } catch (const std::exception& e) {
            std::cerr << "Error connecting to server: " << e.what() << std::endl;
            display_queue.push(Message(false));
            continue;  // Retry connection
        }

        display_queue.push(Message(true));  // Connection request successful
        return true;                        // Connection successful
    }
    return false;  // If we exit the loop without a connection request
}

void ConnectionHandler::setup_communication() {
    // Feed the input queue to the sender
    sender = std::make_unique<ClientSender>(protocol, ingame_queue);
    sender->start();

    // Feed the receiver queue to the display
    receiver = std::make_unique<ClientReceiver>(protocol, display_queue);
    receiver->start();
}

void ConnectionHandler::wait_for_game_start() {
    while (should_keep_running()) {
        Message msg = pregame_queue.pop();
        ingame_queue.push(msg);

        switch (msg.get_type()) {
            case MessageType::CREATE_GAME_CMD:
                player_name = msg.get_content<CreateGameCommand>().get_player_name();
                display_queue.push(Message(true));
                return;  // Exit the loop when CREATE_GAME_CMD is received
            case MessageType::JOIN_GAME_CMD:
                player_name = msg.get_content<JoinGameCommand>().get_player_name();
                display_queue.push(Message(true));
                return;  // Exit the loop when JOIN_GAME_CMD is received
            default:
                continue;
        }
    }
}

void ConnectionHandler::stop() { Thread::stop(); }

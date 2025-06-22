#include "connection_handler.h"

#include <iostream>
#include <memory>
#include <string>

#include "common/message.h"

#include "protocol.h"
#include "receiver.h"
#include "sender.h"

ConnectionHandler::ConnectionHandler(Queue<Message>& lobby_input_queue,
                                     Queue<Message>& lobby_output_queue,
                                     Queue<Message>& game_input_queue,
                                     Queue<Message>& game_output_queue):
        lobby_input_queue(lobby_input_queue),
        lobby_output_queue(lobby_output_queue),
        game_input_queue(game_input_queue),
        game_output_queue(game_output_queue),
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
            msg = lobby_output_queue.pop();
        } catch (const ClosedQueue&) {
            std::cerr << "ConnectionHandler: Input queue closed, stopping." << std::endl;
            return false;
        }

        if (msg.get_type() != MessageType::CONN_REQ)
            continue;

        auto conn_req = msg.get_content<ConnectionRequest>();
        try {
            auto socket = std::make_shared<Socket>(conn_req.get_ip().c_str(),
                                                   conn_req.get_port().c_str());
            protocol = std::make_shared<ClientProtocol>(socket);
        } catch (const std::exception& e) {
            std::cerr << "Error connecting to server: " << e.what() << std::endl;
            lobby_input_queue.push(
                    Message(ErrorResponse("Connection failed: " + std::string(e.what()))));
            continue;  // Retry connection
        }

        lobby_input_queue.push(Message(ConnectionResponse()));  // Connection request successful
        return true;                                            // Connection successful
    }
    return false;  // If we exit the loop without a connection request
}

void ConnectionHandler::setup_communication() {
    // Feed the input queue to the sender
    sender = std::make_unique<ClientSender>(protocol, game_input_queue);
    sender->start();

    // Feed the receiver queue to the display
    receiver = std::make_unique<ClientReceiver>(protocol, game_output_queue);
    receiver->start();
}

void ConnectionHandler::wait_for_game_start() {
    while (should_keep_running()) {
        Message msg = lobby_output_queue.pop();
        game_input_queue.push(msg);

        // wait for response
        auto response = game_output_queue.pop();
        lobby_input_queue.push(response);

        switch (msg.get_type()) {
            case MessageType::CREATE_GAME_CMD:
                player_name = msg.get_content<CreateGameCommand>().get_player_name();
                break;
            case MessageType::JOIN_GAME_CMD:
                player_name = msg.get_content<JoinGameCommand>().get_player_name();
                break;
            default:
                continue;
        }

        switch (response.get_type()) {
            case MessageType::ERROR_RESP:
                continue;  // Retry
            default:
                return;  // Game created/joined successfully
        }
    }
}

void ConnectionHandler::stop() { Thread::stop(); }

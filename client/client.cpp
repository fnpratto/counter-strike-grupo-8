#include "client.h"

#include <atomic>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <arpa/inet.h>

#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"

// Default to GUI if not defined
// Actually not necessary just better for IntelliSense
#ifndef UI_TYPE_GUI
#ifndef UI_TYPE_TUI
#define UI_TYPE_GUI
#endif  // !UI_TYPE_TUI
#endif  // !UI_TYPE_GUI

#ifdef UI_TYPE_GUI
#include "qt_display.h"
#include "sdl_display.h"
#elif defined(UI_TYPE_TUI)
#include "text_display.h"
#endif

#include "receiver.h"
#include "sender.h"

Client::Client():
#ifdef UI_TYPE_GUI
        display(std::make_unique<QtDisplay>(display_queue, pregame_queue)),
#elif defined(UI_TYPE_TUI)
        display(std::make_unique<TextDisplay>(display_queue, pregame_queue)),
#endif
        sender(nullptr),
        receiver(nullptr) {
    display->start();
}

void Client::run() {
    if (connect_to_server()) {
        setup_communication();
        wait_for_game_start();
        switch_display();

        while (display->is_alive() && sender->is_alive() && receiver->is_alive())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    cleanup();
}

bool Client::connect_to_server() {
    Message msg;
    while (true) {
        msg = pregame_queue.pop();

        if (msg.get_type() != MessageType::CONN_REQ) {
            if (!display->is_alive()) {
                display->stop();
                display->join();
                return false;
            }
            continue;
        }

        auto conn_req = msg.get_content<ConnectionRequest>();

        try {
            protocol = std::make_shared<ClientProtocol>(
                    Socket(conn_req.get_ip().c_str(), conn_req.get_port().c_str()));
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            continue;
        }

        break;
    }

    std::cout << "Connected to server" << std::endl;
    return true;
}

void Client::setup_communication() {
    // Feed the input queue to the sender
    sender = std::make_unique<ClientSender>(protocol, ingame_queue);
    sender->start();

    // Feed the receiver queue to the display
    receiver = std::make_unique<ClientReceiver>(protocol, display_queue);
    receiver->start();
}

void Client::wait_for_game_start() {
    Message msg;
    while (true) {
        msg = pregame_queue.pop();
        ingame_queue.push(msg);

        if (msg.get_type() == MessageType::JOIN_GAME_CMD ||
            msg.get_type() == MessageType::CREATE_GAME_CMD)
            break;
    }
}

void Client::switch_display() {
#ifdef UI_TYPE_GUI
    display->stop();
    display->join();
    display = std::make_unique<SDLDisplay>(display_queue, ingame_queue);
    display->start();
    std::cout << "Switched to SDLDisplay" << std::endl;
#elif defined(UI_TYPE_TUI)
    display->stop();
    display->join();
    display = std::make_unique<TextDisplay>(display_queue, ingame_queue);
    display->start();
#endif
}

void Client::cleanup() {
    if (display) {
        display->stop();
        display->join();
    }
    if (sender) {
        sender->stop();
        sender->join();
    }
    if (receiver) {
        receiver->stop();
        receiver->join();
    }

    // TODO: this is all happy path code, we should handle non happy paths (i.e. uninitialized
    // threads)
}

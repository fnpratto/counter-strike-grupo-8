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

#include "connection_handler.h"
#include "receiver.h"
#include "sender.h"

Client::Client():
#ifdef UI_TYPE_GUI
        lobby_display(std::make_unique<QtDisplay>(display_queue, pregame_queue)),
#elif defined(UI_TYPE_TUI)
        lobby_display(std::make_unique<TextDisplay>(display_queue,
                                                    pregame_queue)),  // TODO esto no compila
#endif
        sender(nullptr),
        receiver(nullptr) {
}

void Client::run() {
    std::string player_name;
    {
        ConnectionHandler connection_handler(pregame_queue, ingame_queue, display_queue);
        connection_handler.start();

        lobby_display->run();
        lobby_display = nullptr;  // Lobby display finished, no longer needed

        connection_handler.stop();
        connection_handler.join();

        sender = connection_handler.get_sender();
        receiver = connection_handler.get_receiver();
        player_name = connection_handler.get_player_name();  // cppcheck-suppress[unreadVariable]
    }

    if (protocol) {
#ifdef UI_TYPE_GUI
        display = std::make_unique<SDLDisplay>(display_queue, ingame_queue, player_name);
        display->start();
        std::cout << "Started SDLDisplay" << std::endl;
#elif defined(UI_TYPE_TUI)
        display = std::make_unique<TextDisplay>(display_queue, ingame_queue);
        display->start();
#endif

        while (display->is_alive() && sender->is_alive() && receiver->is_alive())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    cleanup();
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

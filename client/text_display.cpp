#include "text_display.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "common/message.h"

#include "requests.h"
#include "text_display.h"

TextDisplay::TextDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue):
        Display(input_queue, output_queue) {
// Set stdin to non-blocking
#ifdef _WIN32
    // Windows-specific code
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & ~ENABLE_LINE_INPUT);
#else
    // Unix/Linux/Mac OS X
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (flags == -1)
        perror("fcntl get");
    if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) == -1)
        perror("fcntl set");
#endif
}

void TextDisplay::run() {
    Message message;
    while (should_keep_running()) {
        if (input_queue.try_pop(message))
            draw(message);
        read_input();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void TextDisplay::draw(const Message& message) {
    // Delete later
    if (message.get_type() == MessageType::NONE)
        return;

    throw std::runtime_error("Invalid message type for TextDisplay");
    // TODO: Implement the display logic for different message types
    // if (message.get_type() == MessageType::RAW_TEXT) {
    //     std::cout << message.get_content<std::string>() << std::flush;
    // } else {
    // }
}


void TextDisplay::read_input() {
    static char c;

    ssize_t n = ::read(STDIN_FILENO, &c, 1);
    if (n > 0) {
        handle_char(c);
    } else if (n == -1 && errno == EWOULDBLOCK) {
        // no data yet
    } else if (n == 0) {
        stop();
    } else {
        perror("read");
        stop();
    }
}

void TextDisplay::handle_char(char c) {
    if (c != '\n') {
        line.push_back(c);
        return;
    }

    if (line.empty())
        return;

    auto command_msg = parse_line(line);
    if (command_msg.get_type() != MessageType::NONE)
        output_queue.push(command_msg);

    line.clear();
}

Message TextDisplay::parse_line(const std::string& line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;

    if (command == "connect") {
        std::string ip;
        std::string port;
        iss >> ip;
        iss >> port;
        return Message(ConnectionRequest(ip, port));
    } else if (command == "create") {
        std::string game_name;
        iss >> game_name;
        return Message(CreateGameCommand(game_name));
    } else if (command == "join") {
        std::string game_name;
        iss >> game_name;
        return Message(JoinGameCommand(game_name));
    } else if (command == "list") {
        return Message(ListGamesCommand());
    } else {
        return Message();
    }
}

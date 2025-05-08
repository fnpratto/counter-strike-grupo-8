#include "text_input.h"

#include <sstream>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "common/commands.h"

void TextInput::setup() {
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

void TextInput::work() {
    static char c;

    ssize_t n = ::read(STDIN_FILENO, &c, 1);
    if (n > 0) {
        handle_char(c);
        // } else if (n == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
    } else if (n == -1 && (errno == EAGAIN)) {
        // no data yet
    } else if (n == 0) {
        stop();
    } else {
        perror("read");
        stop();
    }
}

void TextInput::handle_char(char c) {
    if (c != '\n') {
        line.push_back(c);
        return;
    }

    if (line.empty())
        return;

    auto command_msg = parse_line(line);
    if (command_msg.get_type() != MessageType::NONE)
        queue.push(command_msg);

    line.clear();
}

Message TextInput::parse_line(const std::string& line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;

    if (command == "crear") {
        std::string game_name;
        iss >> game_name;
        return Message(CreateGameCommand(game_name));
    } else if (command == "unirse") {
        std::string game_name;
        iss >> game_name;
        return Message(JoinGameCommand(game_name));
    } else if (command == "listar") {
        return Message(ListGamesCommand());
    } else {
        return Message();
    }
}

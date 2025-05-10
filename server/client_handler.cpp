#include "client_handler.h"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "errors.h"
#include "game_monitor.h"

void ClientHandler::run() {
    try {
        while (should_keep_running()) {
            // auto message = protocol.recv();

            // switch (message.get_type()) {
            //     case MessageType::CREATE_GAME_CMD:
            //         handle_create_game(message.get_content<CreateGameCommand>());
            //         break;

            //     case MessageType::JOIN_GAME_CMD:
            //         handle_join_game(message.get_content<JoinGameCommand>());
            //         break;

            //     case MessageType::LIST_GAMES_CMD:
            //         handle_list_games(message.get_content<ListGamesCommand>());
            //         break;

            //     default:
            //         throw std::runtime_error("Invalid command received");
            //         break;
            // }
        }
    } catch (const GameError& e) {
        std::cerr << e.what() << std::endl;
    } catch (const ServerDisconnectError& e) {
        stop();
        return;
    } catch (const std::exception& e) {
        if (!should_keep_running())
            return;

        std::cerr << "Acceptor thread error: " << e.what() << std::endl;
    }
}

void ClientHandler::stop() {
    Thread::stop();

    if (!protocol.is_closed())
        protocol.close();

    if (game)
        game->finish();
}

void ClientHandler::handle_list_games() {}

void ClientHandler::handle_create_game(const CreateGameCommand& cmd) {
    if (cmd.get_game_name().empty())
        throw ServerError("Game name cannot be empty");
}

void ClientHandler::handle_join_game(const JoinGameCommand& cmd) {
    if (cmd.get_game_name().empty())
        throw ServerError("Game name cannot be empty");
}

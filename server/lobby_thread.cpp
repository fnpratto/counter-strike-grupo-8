#include "lobby_thread.h"

#include <iostream>

LobbyThread::LobbyThread(LobbyMonitor& lobby_monitor, std::shared_ptr<ServerProtocol> proto,
                         std::unique_ptr<Sender>& sender, std::unique_ptr<Receiver>& receiver):
        lobby_monitor(lobby_monitor),
        protocol(std::move(proto)),
        sender(sender),
        receiver(receiver) {}

void LobbyThread::run() {
    while (should_keep_running()) {
        auto msg = protocol->recv();

        // TODO: function map
        switch (msg.get_type()) {
            {
                case MessageType::CREATE_GAME_CMD: {
                    handle_create_game_cmd(msg.get_content<CreateGameCommand>());
                    break;
                }
                case MessageType::JOIN_GAME_CMD: {
                    handle_join_game_cmd(msg.get_content<JoinGameCommand>());
                    break;
                }
                case MessageType::LIST_GAMES_CMD: {
                    handle_list_games_cmd();
                    break;
                }
                default:
                    std::cerr << "Unknown command type: " << static_cast<int>(msg.get_type())
                              << std::endl;
                    break;
            }
        }
    }
}

void LobbyThread::handle_create_game_cmd(const CreateGameCommand& cmd) {
    pipe_t pipe = lobby_monitor.create_game(cmd.get_game_name());

    sender = std::make_unique<Sender>(protocol, std::move(pipe.first));
    receiver = std::make_unique<Receiver>(protocol, std::move(pipe.second));
    sender->start();
    receiver->start();

    stop();
}

void LobbyThread::handle_join_game_cmd(const JoinGameCommand& cmd) {
    pipe_t pipe = lobby_monitor.join_game(cmd.get_game_name());

    sender = std::make_unique<Sender>(protocol, std::move(pipe.first));
    receiver = std::make_unique<Receiver>(protocol, std::move(pipe.second));
    sender->start();
    receiver->start();

    stop();
}

void LobbyThread::handle_list_games_cmd() {
    // auto games = lobby_monitor.get_games_names();
    // TODO: send the list of games to the client
}

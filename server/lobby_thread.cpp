#include "lobby_thread.h"

#include <iostream>
#include <string>

#include "common/responses.h"

LobbyThread::LobbyThread(ServerProtocol& proto, LobbyMonitor& lobby_monitor,
                         std::function<void(const std::string&, pipe_t)> join_callback):
        protocol(proto), lobby_monitor(lobby_monitor), join_callback(std::move(join_callback)) {}

template <>
void LobbyThread::handle(const CreateGameCommand& cmd) {
    pipe_t pipe =
            lobby_monitor.create_game(cmd.get_game_name(), cmd.get_map_id(), cmd.get_player_name());

    join_callback(cmd.get_player_name(), pipe);

    stop();
}

template <>
void LobbyThread::handle(const JoinGameCommand& cmd) {
    pipe_t pipe = lobby_monitor.join_game(cmd.get_game_name(), cmd.get_player_name());

    join_callback(cmd.get_player_name(), pipe);

    stop();
}

template <>
void LobbyThread::handle([[maybe_unused]] const ListMapsCommand& cmd) {
    auto maps = lobby_monitor.get_maps_info();
    protocol.send(Message(ListMapsResponse(maps)));
}

template <>
void LobbyThread::handle([[maybe_unused]] const ListGamesCommand& cmd) {
    auto games = lobby_monitor.get_games_info();
    protocol.send(Message(ListGamesResponse(games)));
}

void LobbyThread::run() {
    try {
        while (should_keep_running()) {
            // Wait for a message from the client
            auto msg = protocol.recv();
            // TODO: function map
            switch (msg.get_type()) {
                case MessageType::CREATE_GAME_CMD: {
                    handle(msg.get_content<CreateGameCommand>());
                    break;
                }
                case MessageType::JOIN_GAME_CMD: {
                    handle(msg.get_content<JoinGameCommand>());
                    break;
                }
                case MessageType::LIST_MAPS_CMD: {
                    handle(msg.get_content<ListMapsCommand>());
                    break;
                }
                case MessageType::LIST_GAMES_CMD: {
                    handle(msg.get_content<ListGamesCommand>());
                    break;
                }
                default:
                    std::cerr << "Unknown command type: " << static_cast<int>(msg.get_type())
                              << std::endl;
                    break;
            }
        }
    } catch (const ServerDisconnectError& e) {
        stop();
        return;
    } catch (const std::exception& e) {
        if (!should_keep_running())
            return;

        std::cerr << "Error: " << e.what() << std::endl;
    }
}

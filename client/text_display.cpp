#include "text_display.h"

#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "common/commands.h"
#include "common/message.h"
#include "common/models.h"
#include "common/utils/vector_2d.h"

#include "requests.h"

std::ostream& operator<<(std::ostream& os, const GunType& type) {
    switch (type) {
        case GunType::Glock:
            return os << "Glock";
        case GunType::AK47:
            return os << "AK-47";
        case GunType::M3:
            return os << "M3";
        case GunType::AWP:
            return os << "AWP";
        default:
            return os << "Unknown Gun";
    }
}

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

    switch (message.get_type()) {
        case MessageType::LIST_GAMES_RESP: {
            auto game_list = message.get_content<ListGamesResponse>().get_games_info();
            std::cout << "Available games:\n";
            for (const auto& game_info: game_list) {
                std::cout << "Game Name: " << game_info.name
                          << ", Players: " << game_info.players_count << ", Status: "
                          << (game_info.phase == PhaseType::WarmUp  ? "WarmUp" :
                              game_info.phase == PhaseType::Buying  ? "Buying" :
                              game_info.phase == PhaseType::Playing ? "Playing" :
                                                                      "Round Finished")
                          << "\n";
            }
            break;
        }
        case MessageType::GAME_UPDATE: {
            const auto& update = message.get_content<GameUpdate>();
            state = state.merged(update);

            std::cout << "Game Update:" << std::endl;
            std::string phase_str =
                    (state.get_phase().get_phase() == PhaseType::Buying)  ? "Buying" :
                    (state.get_phase().get_phase() == PhaseType::Playing) ? "Playing" :
                    (state.get_phase().get_phase() == PhaseType::End)     ? "Round Finished" :
                                                                            "Warm Up";
            std::cout << "Phase: " << phase_str << std::endl;
            std::cout << "Players:" << std::endl;
            for (const auto& [player_name, player]:  // cppcheck-suppress[unassignedVariable]
                 state.get_players()) {
                std::string team_str =
                        (player.get_team() == Team::TT) ? "Terrorist" : "Counter-Terrorist";
                std::cout << " - " << player_name << " (" << team_str << ")" << std::endl;
            }
            break;
        }
        case MessageType::SHOP_PRICES_RESP: {
            const auto& prices = message.get_content<ShopPricesResponse>().get_gun_prices();
            std::cout << "Shop:\n";
            std::cout << "Gun Prices:\n";
            for (const auto& [gun, price]: prices) {  // cppcheck-suppress[unassignedVariable]
                std::cout << " - " << gun << ": $" << price << std::endl;
            }
            std::cout << "Ammo Prices:\n";
            const auto& ammo_prices = message.get_content<ShopPricesResponse>().get_ammo_prices();
            for (const auto& [gun, price]: ammo_prices) {  // cppcheck-suppress[unassignedVariable]
                std::cout << " - " << gun << ": $" << price << std::endl;
            }
            break;
        }
        case MessageType::BOOL: {
            bool result = message.get_content<bool>();
            std::cout << "Operation result: " << (result ? "Success" : "Failure") << std::endl;
            break;
        }
        default:
            throw std::runtime_error("Invalid message type for TextDisplay: " +
                                     std::to_string(static_cast<int>(message.get_type())));
            break;
    }
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

template <>
Message TextDisplay::build_message<ConnectionRequest>(std::istringstream& iss) {
    std::string ip, port;
    iss >> ip >> port;
    return Message(ConnectionRequest(ip, port));
}


template <>
Message TextDisplay::build_message<CreateGameCommand>(std::istringstream& iss) {
    std::string game_name;
    std::string player_name;
    iss >> game_name >> player_name;
    return Message(CreateGameCommand(game_name, player_name));
}

template <>
Message TextDisplay::build_message<JoinGameCommand>(std::istringstream& iss) {
    std::string game_name;
    std::string player_name;
    iss >> game_name >> player_name;
    return Message(JoinGameCommand(game_name, player_name));
}

template <>
Message TextDisplay::build_message<ListGamesCommand>([[maybe_unused]] std::istringstream& iss) {
    return Message(ListGamesCommand());
}

template <>
Message TextDisplay::build_message<SelectTeamCommand>(std::istringstream& iss) {
    std::string team_str;
    iss >> team_str;

    Team team;
    if (team_str == "terrorist" || team_str == "t") {
        team = Team::TT;
    } else if (team_str == "counterterrorist" || team_str == "ct") {
        team = Team::CT;
    } else {
        throw std::invalid_argument("Invalid team: " + team_str);
    }

    return Message(SelectTeamCommand(team));
}

template <>
Message TextDisplay::build_message<StartGameCommand>([[maybe_unused]] std::istringstream& iss) {
    return Message(StartGameCommand());
}

template <>
Message TextDisplay::build_message<BuyGunCommand>(std::istringstream& iss) {
    std::string gun_str;
    iss >> gun_str;

    GunType gun;
    if (gun_str == "glock") {
        gun = GunType::Glock;
    } else if (gun_str == "ak47") {
        gun = GunType::AK47;
    } else if (gun_str == "m3") {
        gun = GunType::M3;
    } else if (gun_str == "awp") {
        gun = GunType::AWP;
    } else {
        throw std::invalid_argument("Invalid gun: " + gun_str);
    }

    return Message(BuyGunCommand(gun));
}

template <>
Message TextDisplay::build_message<MoveCommand>(std::istringstream& iss) {
    std::string direction_str;
    iss >> direction_str;

    Vector2D direction(0, 0);
    if (direction_str == "up") {
        direction = Vector2D(0, 1);
    } else if (direction_str == "down") {
        direction = Vector2D(0, -1);
    } else if (direction_str == "left") {
        direction = Vector2D(-1, 0);
    } else if (direction_str == "right") {
        direction = Vector2D(1, 0);
    } else {
        throw std::invalid_argument("Invalid direction: " + direction_str);
    }

    return Message(MoveCommand(direction));
}

template <>
Message TextDisplay::build_message<StopPlayerCommand>([[maybe_unused]] std::istringstream& iss) {
    return Message(StopPlayerCommand());
}

template <>
Message TextDisplay::build_message<AimCommand>(std::istringstream& iss) {
    float x, y;
    iss >> x >> y;
    return Message(AimCommand(Vector2D(x, y)));
}

template <>
Message TextDisplay::build_message<AttackCommand>([[maybe_unused]] std::istringstream& iss) {
    return Message(AttackCommand());
}

template <>
Message TextDisplay::build_message<ReloadCommand>([[maybe_unused]] std::istringstream& iss) {
    return Message(ReloadCommand());
}

template <>
Message TextDisplay::build_message<SwitchItemCommand>(std::istringstream& iss) {
    std::string slot_str;
    iss >> slot_str;

    ItemSlot slot;
    if (slot_str == "primary") {
        slot = ItemSlot::Primary;
    } else if (slot_str == "secondary") {
        slot = ItemSlot::Secondary;
    } else if (slot_str == "melee") {
        slot = ItemSlot::Melee;
    } else if (slot_str == "bomb") {
        slot = ItemSlot::Bomb;
    } else {
        throw std::invalid_argument("Invalid gun slot: " + slot_str);
    }

    return Message(SwitchItemCommand(slot));
}

template <>
Message TextDisplay::build_message<PlantBombCommand>([[maybe_unused]] std::istringstream& iss) {
    return Message(PlantBombCommand());
}

template <>
Message TextDisplay::build_message<DefuseBombCommand>([[maybe_unused]] std::istringstream& iss) {
    return Message(DefuseBombCommand());
}

template <>
Message TextDisplay::build_message<PickUpItemCommand>([[maybe_unused]] std::istringstream& iss) {
    return Message(PickUpItemCommand());
}
template <>
Message TextDisplay::build_message<GetShopPricesCommand>([[maybe_unused]] std::istringstream& iss) {
    return Message(GetShopPricesCommand());
}

template <>
Message TextDisplay::build_message<GetScoreboardCommand>([[maybe_unused]] std::istringstream& iss) {
    return Message(GetScoreboardCommand());
}

template <>
Message TextDisplay::build_message<LeaveGameCommand>([[maybe_unused]] std::istringstream& iss) {
    return Message(LeaveGameCommand());
}

Message TextDisplay::parse_line(const std::string& line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;


    using CommandFunction = std::function<Message(std::istringstream&)>;
    static const std::unordered_map<std::string, CommandFunction> command_map = {
            {"connect",
             [this](std::istringstream& is) { return this->build_message<ConnectionRequest>(is); }},
            {"create",
             [this](std::istringstream& is) { return this->build_message<CreateGameCommand>(is); }},
            {"join",
             [this](std::istringstream& is) { return this->build_message<JoinGameCommand>(is); }},
            {"list",
             [this](std::istringstream& is) { return this->build_message<ListGamesCommand>(is); }},
            {"team",
             [this](std::istringstream& is) { return this->build_message<SelectTeamCommand>(is); }},
            {"start",
             [this](std::istringstream& is) { return this->build_message<StartGameCommand>(is); }},
            {"buy",
             [this](std::istringstream& is) { return this->build_message<BuyGunCommand>(is); }},
            {"move",
             [this](std::istringstream& is) { return this->build_message<MoveCommand>(is); }},
            {"stop",
             [this](std::istringstream& is) { return this->build_message<StopPlayerCommand>(is); }},
            {"aim", [this](std::istringstream& is) { return this->build_message<AimCommand>(is); }},
            {"shoot",
             [this](std::istringstream& is) { return this->build_message<AttackCommand>(is); }},
            {"reload",
             [this](std::istringstream& is) { return this->build_message<ReloadCommand>(is); }},
            {"switch",
             [this](std::istringstream& is) { return this->build_message<SwitchItemCommand>(is); }},
            {"plant",
             [this](std::istringstream& is) { return this->build_message<PlantBombCommand>(is); }},
            {"defuse",
             [this](std::istringstream& is) { return this->build_message<DefuseBombCommand>(is); }},
            {"pickup",
             [this](std::istringstream& is) { return this->build_message<PickUpItemCommand>(is); }},
            {"leave",
             [this](std::istringstream& is) { return this->build_message<LeaveGameCommand>(is); }},
            {"shop",
             [this](std::istringstream& is) {
                 return this->build_message<GetShopPricesCommand>(is);
             }},
            {"scoreboard",
             [this](std::istringstream& is) {
                 return this->build_message<GetScoreboardCommand>(is);
             }},
    };

    auto it = command_map.find(command);
    if (it != command_map.end())
        return it->second(iss);

    return Message();
}

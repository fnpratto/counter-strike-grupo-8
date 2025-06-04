#include "game.h"

#include <utility>

#include "server/errors.h"
#include "server/player_message.h"

Game::Game(const std::string& name, std::shared_ptr<Clock>&& game_clock, Map&& map):
        Logic<GameState, GameUpdate>(GameState(std::move(game_clock))),
        name(name),
        physics_system(std::move(map), state.get_players()) {}

std::vector<PlayerMessage> Game::tick(const std::vector<PlayerMessage>& msgs) {
    state.clear_updates();
    for (const PlayerMessage& msg: msgs) handle_msg(msg.get_message(), msg.get_player_name());
    advance_players_movement();
    advance_round_logic();

    GameUpdate update = state.get_updates();
    if (update.has_change())
        for (const auto& [player_name, player]: state.get_players())
            output_messages.push_back(PlayerMessage(player_name, Message(update)));

    return output_messages;
}

std::string Game::get_name() const { return name; }

int Game::get_player_count() const { return static_cast<int>(state.get_players().size()); }

PhaseType Game::get_phase() { return state.get_phase().get_type(); }

void Game::advance_round_logic() {
    auto& phase = state.get_phase();

    phase.advance();

    if (phase.is_round_finished())
        advance_round();

    if (state.get_num_rounds() == GameConfig::max_rounds / 2)
        swap_teams();
}

void Game::advance_round() { state.set_num_rounds(state.get_num_rounds() + 1); }

void Game::advance_players_movement() {
    for (const auto& [_, player]: state.get_players()) {  // cppcheck-suppress[unusedVariable]
        if (!player->is_moving())
            continue;

        Vector2D old_pos = player->get_pos();
        Vector2D step = physics_system.calculate_step(player->get_move_dir());
        Vector2D new_pos = old_pos + step;
        // TODO: Check collisions with physics_system (with tiles and entities)
        player->move_to_pos(new_pos);
    }
}

void Game::join_player(const std::string& player_name) {
    if (player_name.empty())
        throw InvalidPlayerNameError();
    if (player_is_in_game(player_name) || is_full() || state.get_phase().is_started())
        throw JoinGameError();

    Team default_team = (state.get_num_tts() > state.get_num_cts()) ? Team::CT : Team::TT;
    if (default_team == Team::TT) {
        Vector2D pos = physics_system.random_spawn_tt_pos();
        state.add_player(player_name, std::make_unique<Player>(default_team, pos));
    } else {
        Vector2D pos = physics_system.random_spawn_ct_pos();
        state.add_player(player_name, std::make_unique<Player>(default_team, pos));
    }

    output_messages.emplace_back(player_name, Message(state.get_full_update()));
    for (const auto& [name, player]: state.get_players()) {
        if (name == player_name)
            continue;
        output_messages.emplace_back(name, Message(state.get_updates()));
    }
}

template <>
void Game::handle<SelectTeamCommand>(const std::string& player_name, const SelectTeamCommand& msg) {
    const std::unique_ptr<Player>& player = state.get_player(player_name);
    if (state.get_phase().is_started())
        throw SelectTeamError();
    if (team_is_full(msg.get_team()))
        // TODO: Return error response
        return;

    Team old_team = player->is_tt() ? Team::TT : Team::CT;

    if (old_team == msg.get_team())
        return;

    player->select_team(msg.get_team());
}

template <>
void Game::handle<StartGameCommand>(const std::string& player_name,
                                    [[maybe_unused]] const StartGameCommand& msg) {
    if (state.get_phase().is_started())
        throw StartGameError();

    auto& player = state.get_player(player_name);
    player->set_ready();
    if (all_players_ready()) {
        give_bomb_to_random_tt();
        state.get_phase().start_buying_phase();
    }
}

template <>
void Game::handle<BuyGunCommand>(const std::string& player_name, const BuyGunCommand& msg) {
    auto& player = state.get_player(player_name);
    if (!state.get_phase().is_buying_phase())
        throw BuyGunError();
    // if (physics_system.player_not_in_spawn(player))
    //     return;

    shop.buy_gun(player->get_inventory(), msg.get_gun());
}

// TODO this could take a WeaponSlot instead of GunType
template <>
void Game::handle<BuyAmmoCommand>(const std::string& player_name, const BuyAmmoCommand& msg) {
    auto& player = state.get_player(player_name);
    if (!state.get_phase().is_buying_phase())
        throw BuyAmmoError();
    // if (physics_system.player_not_in_spawn(player))
    //     return;
    WeaponSlot slot =
            (msg.get_gun() == GunType::Glock) ? WeaponSlot::Secondary : WeaponSlot::Primary;
    shop.buy_ammo(player->get_inventory(), slot);
}

template <>
void Game::handle<MoveCommand>(const std::string& player_name, const MoveCommand& msg) {
    if (state.get_phase().is_buying_phase())
        return;

    auto& player = state.get_player(player_name);

    player->start_moving(msg.get_direction());
}

template <>
void Game::handle<StopPlayerCommand>(const std::string& player_name,
                                     [[maybe_unused]] const StopPlayerCommand& msg) {
    auto& player = state.get_player(player_name);
    player->stop_moving();
}

template <>
void Game::handle<AimCommand>(const std::string& player_name, const AimCommand& msg) {
    auto& player = state.get_player(player_name);

    player->aim(msg.get_x(), msg.get_y());
}

template <>
void Game::handle<GetShopPricesCommand>(const std::string& player_name,
                                        [[maybe_unused]] const GetShopPricesCommand& msg) {
    output_messages.push_back(PlayerMessage(
            player_name,
            Message(ShopPricesResponse(shop.get_gun_prices(), shop.get_ammo_prices()))));
}

// void Game::handle_shoot_msg(const std::string& player_name, int x, int y) {
//     if (player->get_current_weapon() == WeaponSlot::Melee) {
//         Knife knife = player->attack();
//         Vector2D pos(player->get_pos_x(), player->get_pos_y());
//         physics_system.handle_attack(knife, pos);
//     } else {
//         std::vector<Bullet> bullets = player->shoot_gun(x, y, physics_system.time_now());
//         for (Bullet& b : bullets)
//             physics_system.shoot_bullet(b);
//     }
// }

template <>
void Game::handle<SwitchWeaponCommand>(const std::string& player_name,
                                       const SwitchWeaponCommand& msg) {
    auto& player = state.get_player(player_name);
    player->equip_weapon(msg.get_slot());
}

template <>
void Game::handle<ReloadCommand>(const std::string& player_name,
                                 [[maybe_unused]] const ReloadCommand& msg) {
    auto& player = state.get_player(player_name);
    player->reload();
}

// TODO function map
void Game::handle_msg(const Message& msg, const std::string& player_name) {
    MessageType msg_type = msg.get_type();
    std::cout << "Game::handle_msg: Received message of type: " << static_cast<int>(msg_type)
              << " from player: " << player_name << "\n";
    switch (msg_type) {
        case MessageType::SELECT_TEAM_CMD:
            handle(player_name, msg.get_content<SelectTeamCommand>());
            break;
        case MessageType::START_GAME_CMD:
            handle(player_name, msg.get_content<StartGameCommand>());
            break;
        case MessageType::BUY_GUN_CMD:
            handle(player_name, msg.get_content<BuyGunCommand>());
            break;
        case MessageType::BUY_AMMO_CMD:
            handle(player_name, msg.get_content<BuyAmmoCommand>());
            break;
        case MessageType::MOVE_CMD:
            handle(player_name, msg.get_content<MoveCommand>());
            break;
        case MessageType::STOP_PLAYER_CMD:
            handle(player_name, msg.get_content<StopPlayerCommand>());
            break;
        case MessageType::AIM_CMD:
            handle(player_name, msg.get_content<AimCommand>());
            break;
        // case MessageType::SHOOT_CMD:
        //     handle(player_name, msg.get_content<ShootCommand>());
        //     break;
        case MessageType::SWITCH_WEAPON_CMD:
            handle(player_name, msg.get_content<SwitchWeaponCommand>());
            break;
        case MessageType::RELOAD_CMD:
            handle(player_name, msg.get_content<ReloadCommand>());
            break;
        case MessageType::GET_SHOP_PRICES_CMD:
            handle(player_name, msg.get_content<GetShopPricesCommand>());
            break;
        default:
            throw std::runtime_error("Invalid message type: " +
                                     std::to_string(static_cast<int>(msg_type)));
            break;
    }
}

void Game::give_bomb_to_random_tt() {
    if (state.get_num_tts() == 0)
        return;

    std::vector<std::string> tt_names;

    for (auto& [player_name, player]: state.get_players()) {
        if (player->is_tt())
            tt_names.push_back(player_name);
    }

    int random_index = rand() % state.get_num_tts();

    std::string player_name = tt_names[random_index];
    auto& player = state.get_player(player_name);
    player->pick_bomb();
}

void Game::swap_teams() {
    for (auto& [_, player]: state.get_players()) {  // cppcheck-suppress[unusedVariable]
        if (player->is_tt()) {
            player->select_team(Team::CT);
        } else if (player->is_ct()) {
            player->select_team(Team::TT);
        }
    }
}

Game::~Game() {}

bool Game::player_is_in_game(const std::string& player_name) const {
    return state.get_players().find(player_name) != state.get_players().end();
}

bool Game::is_full() const {
    return static_cast<int>(state.get_players().size()) == GameConfig::max_players;
}

bool Game::team_is_full(const Team& team) const {
    if (team == Team::TT)
        return state.get_num_tts() == GameConfig::max_team_players;
    return state.get_num_cts() == GameConfig::max_team_players;
}

bool Game::all_players_ready() const {
    for (auto& [_, player]: state.get_players()) {  // cppcheck-suppress[unusedVariable]
        if (!player->is_ready())
            return false;
    }
    return true;
}

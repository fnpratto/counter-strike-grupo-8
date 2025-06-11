#include "game.h"

#include <optional>
#include <utility>

#include "common/scoreboard/scoreboard_entry.h"
#include "server/attack_effects/attack_effect.h"
#include "server/errors.h"
#include "server/player_message.h"
#include "server/weapons/bomb.h"

#include "target_type.h"

Game::Game(const std::string& name, std::shared_ptr<Clock>&& game_clock, Map&& map):
        Logic<GameState, GameUpdate>(GameState(std::move(game_clock), map.get_max_players())),
        name(name),
        physics_system(std::move(map), state.get_players()) {}

bool Game::is_full() const { return state.game_is_full(); }

std::vector<PlayerMessage> Game::tick(const std::vector<PlayerMessage>& msgs) {
    state.clear_updates();

    advance_round_logic();
    for (const PlayerMessage& msg: msgs) handle_msg(msg.get_message(), msg.get_player_name());
    perform_attacks();
    advance_players_movement();

    GameUpdate update = state.get_updates();
    if (update.has_change())
        for (const auto& [player_name, player]: state.get_players())
            output_messages.push_back(PlayerMessage(player_name, Message(update)));

    return std::move(output_messages);
}

std::string Game::get_name() const { return name; }

int Game::get_player_count() const { return static_cast<int>(state.get_players().size()); }

PhaseType Game::get_phase() { return state.get_phase().get_type(); }

void Game::advance_round_logic() {
    auto& phase = state.get_phase();

    phase.advance();

    if (phase.round_has_finished()) {
        state.advance_round();
        for (const auto& [p_name, _]: state.get_players()) move_player_to_spawn(p_name);
    }
    if (state.get_num_rounds() == GameConfig::max_rounds / 2)
        state.swap_players_teams();
}

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

void Game::perform_attacks() {
    if (!state.get_phase().is_playing_phase())
        return;
    for (const auto& [player_name, player]: state.get_players()) {
        auto effects = player->attack(state.get_phase().get_time_now());
        if (effects.empty())
            continue;

        for (const auto& effect: effects) {
            auto closest_target = physics_system.get_closest_target(player_name, effect->get_dir(),
                                                                    effect->get_max_range());
            if (!closest_target.has_value())
                continue;

            bool is_hit = false;
            if (closest_target.value().is_player()) {
                auto& target_player = closest_target.value().get_player().get();
                is_hit = effect->apply(target_player);
                if (target_player->is_dead()) {
                    player->add_kill();
                    player->add_rewards(ScoresConfig::kill, BonificationsConfig::kill);

                    auto gun = target_player->drop_primary_weapon();
                    if (gun.has_value())
                        state.add_dropped_gun(std::move(gun.value()), target_player->get_pos());
                    auto bomb = target_player->drop_bomb();
                    if (bomb.has_value())
                        state.add_bomb(std::move(bomb.value()), target_player->get_pos());
                }
            }

            HitResponse hit_response(player->get_pos(), closest_target.value().get_pos(),
                                     effect->get_dir(), is_hit);
            for (const auto& [p_name, _]: state.get_players())
                output_messages.emplace_back(p_name, Message(hit_response));
        }
    }
}

void Game::join_player(const std::string& player_name) {
    if (player_name.empty())
        throw InvalidPlayerNameError();
    if (state.player_is_in_game(player_name))
        throw JoinGameError();
    if (is_full() || state.get_phase().is_started())
        return;

    Team default_team = (state.get_num_tts() > state.get_num_cts()) ? Team::CT : Team::TT;
    if (default_team == Team::TT) {
        Vector2D pos = physics_system.random_spawn_tt_pos();
        state.add_player(player_name, std::make_unique<Player>(default_team, pos));
    } else {
        Vector2D pos = physics_system.random_spawn_ct_pos();
        state.add_player(player_name, std::make_unique<Player>(default_team, pos));
    }

    output_messages.emplace_back(player_name, Message(state.get_full_update()));
    for (const auto& [p_name, _]: state.get_players()) {
        if (p_name == player_name)
            continue;
        output_messages.emplace_back(p_name, Message(state.get_updates()));
    }
}

template <>
void Game::handle<SelectTeamCommand>(const std::string& player_name, const SelectTeamCommand& msg) {
    if (state.get_phase().is_started())
        throw SelectTeamError();
    if (state.team_is_full(msg.get_team()))
        // TODO: Error response
        return;

    auto& player = state.get_player(player_name);
    player->select_team(msg.get_team());
    move_player_to_spawn(player_name);
}

template <>
void Game::handle<GetCharactersCommand>(const std::string& player_name,
                                        [[maybe_unused]] const GetCharactersCommand& msg) {
    auto& player = state.get_player(player_name);
    std::vector<CharacterType> characters;
    if (player->is_ct()) {
        characters = {CharacterType::Seal_Force, CharacterType::German_GSG_9, CharacterType::UK_SAS,
                      CharacterType::French_GIGN};
    } else if (player->is_tt()) {
        characters = {CharacterType::Pheonix, CharacterType::L337_Krew,
                      CharacterType::Artic_Avenger, CharacterType::Guerrilla};
    }
    output_messages.emplace_back(player_name, Message(CharactersResponse(std::move(characters))));
}

template <>
void Game::handle<SelectCharacterCommand>(const std::string& player_name,
                                          const SelectCharacterCommand& msg) {
    if (state.get_phase().is_started())
        // throw SelectCharacterError();
        return;  // TODO: Error response

    auto& player = state.get_player(player_name);
    player->select_character(msg.get_character_type());
}

template <>
void Game::handle<StartGameCommand>(const std::string& player_name,
                                    [[maybe_unused]] const StartGameCommand& msg) {
    if (state.get_phase().is_started())
        throw StartGameError();

    state.get_player(player_name)->set_ready();
    if (state.all_players_ready()) {
        give_bomb_to_random_tt();
        state.get_phase().start_buying_phase();
        for (const auto& [p_name, _]: state.get_players()) move_player_to_spawn(p_name);
    }
}

template <>
void Game::handle<BuyGunCommand>(const std::string& player_name, const BuyGunCommand& msg) {
    if (!state.get_phase().is_buying_phase())
        return;
    if (!physics_system.player_in_spawn(player_name))
        return;

    auto& player = state.get_player(player_name);
    auto gun = player->drop_primary_weapon();
    if (gun.has_value())
        state.add_dropped_gun(std::move(gun.value()), player->get_pos());

    shop.buy_gun(player->get_inventory(), msg.get_gun());
}

template <>
void Game::handle<BuyAmmoCommand>(const std::string& player_name, const BuyAmmoCommand& msg) {
    if (!state.get_phase().is_buying_phase())
        return;
    if (!physics_system.player_in_spawn(player_name))
        return;

    auto& player = state.get_player(player_name);
    shop.buy_ammo(player->get_inventory(), msg.get_slot());
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
    player->aim(msg.get_direction());
}

template <>
void Game::handle<GetShopPricesCommand>(const std::string& player_name,
                                        [[maybe_unused]] const GetShopPricesCommand& msg) {
    output_messages.emplace_back(player_name, Message(ShopPricesResponse(shop.get_gun_prices(),
                                                                         shop.get_ammo_prices())));
}

template <>
void Game::handle<AttackCommand>(const std::string& player_name,
                                 [[maybe_unused]] const AttackCommand& msg) {
    auto& player = state.get_player(player_name);
    player->start_attacking();
}

template <>
void Game::handle<SwitchItemCommand>(const std::string& player_name, const SwitchItemCommand& msg) {
    auto& player = state.get_player(player_name);
    player->equip_item(msg.get_slot());
}

template <>
void Game::handle<ReloadCommand>(const std::string& player_name,
                                 [[maybe_unused]] const ReloadCommand& msg) {
    auto& player = state.get_player(player_name);
    player->reload();
}

template <>
void Game::handle<GetScoreboardCommand>(const std::string& player_name,
                                        [[maybe_unused]] const GetScoreboardCommand& msg) {
    std::map<std::string, ScoreboardEntry> scoreboard;
    for (const auto& [p_name, player]: state.get_players())
        scoreboard.emplace(p_name, player->get_scoreboard_entry());
    output_messages.emplace_back(player_name, Message(ScoreboardResponse(std::move(scoreboard))));
}

// TODO function map
void Game::handle_msg(const Message& msg, const std::string& player_name) {
    if (state.get_player(player_name)->is_dead())
        return;
    MessageType msg_type = msg.get_type();
    std::cout << "Game::handle_msg: Received message of type: " << static_cast<int>(msg_type)
              << " from player: " << player_name << "\n";
    switch (msg_type) {
        case MessageType::SELECT_TEAM_CMD:
            handle(player_name, msg.get_content<SelectTeamCommand>());
            break;
        case MessageType::GET_CHARACTERS_CMD:
            handle(player_name, msg.get_content<GetCharactersCommand>());
            break;
        case MessageType::SELECT_CHARACTER_CMD:
            handle(player_name, msg.get_content<SelectCharacterCommand>());
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
        case MessageType::ATTACK_CMD:
            handle(player_name, msg.get_content<AttackCommand>());
            break;
        case MessageType::SWITCH_ITEM_CMD:
            handle(player_name, msg.get_content<SwitchItemCommand>());
            break;
        case MessageType::RELOAD_CMD:
            handle(player_name, msg.get_content<ReloadCommand>());
            break;
        case MessageType::GET_SHOP_PRICES_CMD:
            handle(player_name, msg.get_content<GetShopPricesCommand>());
            break;
        case MessageType::GET_SCOREBOARD_CMD:
            handle(player_name, msg.get_content<GetScoreboardCommand>());
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
    player->pick_bomb(Bomb());
}

void Game::move_player_to_spawn(const std::string& player_name) {
    auto& player = state.get_player(player_name);
    if (player->is_tt())
        player->move_to_pos(physics_system.random_spawn_tt_pos());
    else
        player->move_to_pos(physics_system.random_spawn_ct_pos());
}

Game::~Game() {}

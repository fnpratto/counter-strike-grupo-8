#include "game.h"

#include <optional>
#include <utility>

#include "common/game/scoreboard_entry.h"
#include "server/attack_effects/attack_effect.h"
#include "server/errors.h"
#include "server/physics/target_type.h"
#include "server/player_message.h"
#include "server/weapons/bomb.h"

Game::Game(const std::string& name, std::shared_ptr<Clock>&& game_clock, Map&& map):
        Logic<GameState, GameUpdate>(GameState(std::move(game_clock), map.max_players)),
        name(name),
        physics_system(std::move(map), state.get_players()) {}

bool Game::is_full() const { return state.game_is_full(); }

std::vector<PlayerMessage> Game::tick(const std::vector<PlayerMessage>& msgs) {
    if (state.get_phase().is_game_end())
        return {};

    state.clear_updates();

    advance_round_logic();
    for (const PlayerMessage& msg: msgs) handle_msg(msg.get_message(), msg.get_player_name());
    perform_attacks();
    advance_players_movement();

    GameUpdate update = state.get_updates();
    if (update.has_change())
        send_msg_to_all_players(Message(update));

    return std::move(output_messages);
}

std::string Game::get_name() const { return name; }

int Game::get_player_count() const { return static_cast<int>(state.get_players().size()); }

PhaseType Game::get_phase() { return state.get_phase().get_type(); }

void Game::advance_round_logic() {
    auto& phase = state.get_phase();
    bool phase_change = phase.advance();
    if (phase_change) {
        if (phase.is_round_end()) {
            Team winning_team = state.get_winning_team();
            for (const auto& [p_name, player]: state.get_players()) {
                if ((winning_team == Team::TT && player->is_tt()) ||
                    (winning_team == Team::CT && player->is_ct()))
                    player->add_rewards(Scores::win, Bonifications::win);
                else
                    player->add_rewards(Scores::loss, Bonifications::loss);
            }
            send_msg_to_all_players(Message(RoundEndResponse(winning_team)));
        } else if (phase.is_buying_phase()) {
            prepare_new_round();
        }
    }

    if (state.get_num_rounds() == GameConfig::max_rounds / 2)
        state.swap_players_teams();

    if (state.get_num_rounds() == GameConfig::max_rounds)
        phase.end_game();
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
    for (const auto& [p_name, player]: state.get_players()) {
        auto effects = player->attack(state.get_phase().get_time_now());
        if (effects.empty())
            continue;

        for (const auto& effect: effects) {
            auto closest_target = physics_system.get_closest_target(p_name, effect->get_dir(),
                                                                    effect->get_max_range());
            if (!closest_target.has_value())
                continue;

            bool is_hit = apply_attack_effect(player, effect, closest_target.value());

            HitResponse hit_response(player->get_pos(), closest_target.value().get_pos(),
                                     effect->get_dir(), is_hit);
            send_msg_to_all_players(Message(hit_response));
        }
    }
}

bool Game::apply_attack_effect(const std::unique_ptr<Player>& attacker,
                               const std::unique_ptr<AttackEffect>& effect, const Target& target) {
    bool is_hit = false;
    if (target.is_player()) {
        auto& target_player = target.get_player().get();
        is_hit = effect->apply(target_player);
        if (target_player->is_dead()) {
            attacker->add_kill();
            attacker->add_rewards(Scores::kill, Bonifications::kill);
            auto gun = target_player->drop_primary_weapon();
            if (gun.has_value())
                state.add_dropped_gun(std::move(gun.value()), target_player->get_pos());
            auto bomb = target_player->drop_bomb();
            if (bomb.has_value())
                state.add_bomb(std::move(bomb.value()), target_player->get_pos());
        }
    }
    return is_hit;
}

void Game::join_player(const std::string& player_name) {
    if (player_name.empty() || state.player_is_in_game(player_name) || is_full() ||
        state.get_phase().is_started())
        return;

    Team default_team = (state.get_num_tts() > state.get_num_cts()) ? Team::CT : Team::TT;
    if (default_team == Team::TT) {
        Vector2D pos = physics_system.random_spawn_tt_pos();
        state.add_player(player_name, std::make_unique<Player>(default_team, pos));
    } else {
        Vector2D pos = physics_system.random_spawn_ct_pos();
        state.add_player(player_name, std::make_unique<Player>(default_team, pos));
    }

    send_msg_to_single_player(player_name, Message(state.get_full_update()));
    for (const auto& [p_name, _]: state.get_players()) {
        if (p_name == player_name)
            continue;
        send_msg_to_single_player(p_name, Message(state.get_updates()));
    }
}

template <>
void Game::handle<SelectTeamCommand>(const std::string& player_name, const SelectTeamCommand& msg) {
    if (state.get_phase().is_started())
        throw SelectTeamError();
    if (state.team_is_full(msg.get_team())) {
        send_msg_to_single_player(player_name, Message(ErrorResponse()));
        return;
    }

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
                      CharacterType::Arctic_Avenger, CharacterType::Guerrilla};
    }
    send_msg_to_single_player(player_name, Message(CharactersResponse(std::move(characters))));
}

template <>
void Game::handle<SelectCharacterCommand>(const std::string& player_name,
                                          const SelectCharacterCommand& msg) {
    auto& player = state.get_player(player_name);
    player->select_character(msg.get_character_type());
}

template <>
void Game::handle<SetReadyCommand>(const std::string& player_name,
                                   [[maybe_unused]] const SetReadyCommand& msg) {
    if (state.get_phase().is_started())
        throw SetReadyError();

    state.get_player(player_name)->set_ready();
    if (state.all_players_ready()) {
        give_bomb_to_random_tt();
        state.get_phase().start_game();
        for (const auto& [p_name, _]: state.get_players()) move_player_to_spawn(p_name);
    }
}

template <>
void Game::handle<GetShopPricesCommand>(const std::string& player_name,
                                        [[maybe_unused]] const GetShopPricesCommand& msg) {
    send_msg_to_single_player(player_name, Message(ShopPricesResponse(shop.get_gun_prices(),
                                                                      shop.get_ammo_prices())));
}

template <>
void Game::handle<BuyGunCommand>(const std::string& player_name, const BuyGunCommand& msg) {
    auto& player = state.get_player(player_name);
    if (!state.get_phase().is_buying_phase() || !physics_system.player_in_spawn(player_name) ||
        !shop.can_buy_gun(msg.get_gun(), player->get_inventory())) {
        send_msg_to_single_player(player_name, Message(ErrorResponse()));
        return;
    }

    auto gun = player->drop_primary_weapon();
    if (gun.has_value())
        state.add_dropped_gun(std::move(gun.value()), player->get_pos());
    shop.buy_gun(msg.get_gun(), player->get_inventory());
}

template <>
void Game::handle<BuyAmmoCommand>(const std::string& player_name, const BuyAmmoCommand& msg) {
    auto& player = state.get_player(player_name);
    if (!state.get_phase().is_buying_phase() || !physics_system.player_in_spawn(player_name) ||
        !shop.can_buy_ammo(msg.get_slot(), player->get_inventory())) {
        send_msg_to_single_player(player_name, Message(ErrorResponse()));
        return;
    }

    shop.buy_ammo(msg.get_slot(), player->get_inventory());
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
    send_msg_to_single_player(player_name, Message(ScoreboardResponse(std::move(scoreboard))));
}

// TODO: Implement
template <>
void Game::handle<PlantBombCommand>(const std::string& player_name,
                                    [[maybe_unused]] const PlantBombCommand& msg) {
    (void)player_name;
}

// TODO: Implement
template <>
void Game::handle<DefuseBombCommand>(const std::string& player_name,
                                     [[maybe_unused]] const DefuseBombCommand& msg) {
    (void)player_name;
}

// TODO: Implement
template <>
void Game::handle<PickUpItemCommand>(const std::string& player_name,
                                     [[maybe_unused]] const PickUpItemCommand& msg) {
    (void)player_name;
}

// TODO: Implement
template <>
void Game::handle<LeaveGameCommand>(const std::string& player_name,
                                    [[maybe_unused]] const LeaveGameCommand& msg) {
    (void)player_name;
}

#define HANDLE_MSG(command, msg_type)                    \
    case MessageType::msg_type:                          \
        handle(player_name, msg.get_content<command>()); \
        break;

void Game::handle_msg(const Message& msg, const std::string& player_name) {
    if (state.get_player(player_name)->is_dead())
        return;
    MessageType msg_type = msg.get_type();
    switch (msg_type) {
        GAME_COMMANDS_MAP(HANDLE_MSG)
        default:
            throw std::runtime_error("Invalid message type: " +
                                     std::to_string(static_cast<int>(msg_type)));
    }
}

#undef HANDLE_MSG

void Game::give_bomb_to_random_tt() {
    if (state.get_num_tts() == 0)
        return;

    std::vector<std::string> tt_names;

    for (auto& [player_name, player]: state.get_players()) {
        if (player->is_tt())
            tt_names.push_back(player_name);
    }

    int rand_index = rand() % state.get_num_tts();
    std::string player_name = tt_names[rand_index];
    auto& player = state.get_player(player_name);
    // TODO: Player pick bomb stored in state
    player->pick_bomb(Bomb());
}

void Game::prepare_new_round() {
    state.advance_round();
    for (const auto& [p_name, player]: state.get_players()) {
        move_player_to_spawn(p_name);
        reset_for_new_round(player);
        // TODO: Drop player bomb and unplant
    }
    // TODO: Give dropped bomb to random TT
}

void Game::move_player_to_spawn(const std::string& player_name) {
    auto& player = state.get_player(player_name);
    if (player->is_tt())
        player->move_to_pos(physics_system.random_spawn_tt_pos());
    else
        player->move_to_pos(physics_system.random_spawn_ct_pos());
}

void Game::reset_for_new_round(const std::unique_ptr<Player>& player) {
    player->heal();
    player->stop_moving();
    player->stop_attacking();
}

void Game::send_msg_to_all_players(const Message& msg) {
    for (const auto& [p_name, _]: state.get_players()) send_msg_to_single_player(p_name, msg);
}

void Game::send_msg_to_single_player(const std::string& player_name, const Message& msg) {
    output_messages.emplace_back(player_name, msg);
}

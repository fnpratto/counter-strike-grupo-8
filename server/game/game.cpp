#include "game.h"

#include <optional>
#include <utility>

#include "common/game/scoreboard_entry.h"
#include "server/errors.h"
#include "server/items/bomb.h"
#include "server/items/effects/attack_effect.h"
#include "server/physics/target_type.h"
#include "server/player_message.h"

#include "game_config.h"

Game::Game(const std::string& name, std::shared_ptr<Clock>&& game_clock, Map&& map):
        Logic<GameState, GameUpdate>(GameState(std::move(game_clock), map.get_max_players())),
        name(name),
        physics_system(std::move(map), state.get_players(), state.get_dropped_guns(),
                       state.get_bomb()) {}

bool Game::is_full() const { return state.game_is_full(); }

std::vector<PlayerMessage> Game::tick(const std::vector<PlayerMessage>& msgs) {
    if (state.get_phase().is_game_end())
        return {};

    state.clear_updates();

    advance_bomb_logic();
    advance_round_logic();
    for (const PlayerMessage& msg: msgs) handle_msg(msg.get_message(), msg.get_player_name());
    advance_players_movement();
    perform_attacks();

    GameUpdate update = state.get_updates();
    if (update.has_change())
        broadcast(Message(update));

    last_tick = state.get_phase().get_time_now();
    return std::move(output_messages);
}

const std::string& Game::get_name() const { return name; }

std::string Game::get_map_name() const { return physics_system.get_map().get_name(); }

int Game::get_player_count() const { return static_cast<int>(state.get_players().size()); }

PhaseType Game::get_phase() { return state.get_phase().get_type(); }

void Game::advance_round_logic() {
    auto& phase = state.get_phase();
    PhaseType current_phase = phase.get_type();

    if (!phase.is_round_end() && state.is_round_end_condition())
        phase.end_round();
    else
        phase.advance();

    if (current_phase != phase.get_type()) {
        if (phase.is_round_end()) {
            Team winning_team = state.get_winning_team();
            state.give_rewards_to_players(winning_team);
            broadcast(Message(RoundEndResponse(winning_team)));
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
        player->move_to_pos(physics_system.calculate_new_pos(player, get_tick_duration()));
    }
}

void Game::advance_bomb_logic() {
    if (!state.get_phase().is_started())
        return;

    if (!state.get_bomb().has_value()) {
        for (const auto& [_, player]: state.get_players()) {  // cppcheck-suppress[unusedVariable]
            if (player->get_inventory().get_bomb().has_value()) {
                auto& bomb = player->get_inventory().get_bomb().value();
                bomb.advance(state.get_phase().get_time_now());
                if (!bomb.is_planted())
                    return;
                state.add_bomb(std::move(player->drop_bomb().value()), player->get_hitbox().center);
                state.get_phase().start_bomb_planted_phase();
            }
        }
        return;
    }

    auto& bomb = state.get_bomb().value();
    bomb.item.advance(state.get_phase().get_time_now());

    if (bomb.item.is_defused())
        return;

    if (!bomb.item.should_explode())
        return;

    auto effect = bomb.item.explode(bomb.hitbox.get_center());
    auto players_in_explosion =
            physics_system.get_players_in_radius(bomb.hitbox.get_center(), effect.get_max_range());
    for (const auto& player: players_in_explosion) effect.apply(player);
    broadcast(Message(BombExplodedResponse(effect.get_origin(), effect.get_max_range())));
}

void Game::perform_attacks() {
    std::vector<HitResponse> hit_responses;
    for (const auto& [p_name, player]: state.get_players()) {
        auto attack_effects = player->attack(state.get_phase().get_time_now());
        if (attack_effects.empty())
            continue;

        for (const auto& attack_effect: attack_effects) {
            auto closest_target = physics_system.get_closest_target_in_dir(
                    p_name, attack_effect.dir, attack_effect.effect.get_max_range());
            if (!closest_target.has_value()) {
                Vector2D max_hit_pos = attack_effect.effect.get_origin() +
                                       attack_effect.dir * attack_effect.effect.get_max_range();
                hit_responses.push_back(HitResponse(attack_effect.effect.get_origin(), max_hit_pos,
                                                    attack_effect.dir, false));
                continue;
            }

            bool is_hit = false;
            if (state.get_phase().is_playing_phase())
                is_hit = apply_attack_effect(player, attack_effect.effect, closest_target.value());

            hit_responses.push_back(HitResponse(attack_effect.effect.get_origin(),
                                                closest_target.value().get_pos(), attack_effect.dir,
                                                is_hit));
        }
    }

    for (const auto& hit_resp: hit_responses) broadcast(Message(hit_resp));
}

bool Game::apply_attack_effect(const std::unique_ptr<Player>& attacker, const Effect& effect,
                               const Target& target) {
    if (!target.is_player())
        return false;

    auto& target_player = target.get_player();
    bool is_hit = effect.apply(target_player);
    if (target_player->is_dead()) {
        attacker->add_kill();
        attacker->add_rewards(Scores::kill, Bonifications::kill);
        auto gun = target_player->drop_primary_weapon();
        if (gun.has_value())
            state.add_dropped_gun(std::move(gun.value()), target_player->get_hitbox().center);
        auto bomb = target_player->drop_bomb();
        if (bomb.has_value())
            state.add_bomb(std::move(bomb.value()), target_player->get_hitbox().center);
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
        state.add_player(player_name, default_team, pos);
    } else {
        Vector2D pos = physics_system.random_spawn_ct_pos();
        state.add_player(player_name, default_team, pos);
    }

    send_msg(player_name, Message(state.get_full_update()));
    send_msg(player_name, Message(physics_system.get_map()));
    for (const auto& [p_name, _]: state.get_players()) {
        if (p_name == player_name)
            continue;
        send_msg(p_name, Message(state.get_updates()));
    }
}

template <>
void Game::handle<SelectTeamCommand>(const std::string& player_name, const SelectTeamCommand& msg) {
    if (state.get_phase().is_started())
        throw SelectTeamError();
    if (state.team_is_full(msg.get_team())) {
        send_msg(player_name, Message(ErrorResponse()));
        return;
    }

    auto& player = state.get_player(player_name);
    player->select_team(msg.get_team());
    move_player_to_spawn(player);
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
    send_msg(player_name, Message(CharactersResponse(std::move(characters))));
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
        give_bomb_to_random_tt(Bomb());
        state.get_phase().start_game();
        for (const auto& [_, player]: state.get_players())  // cppcheck-suppress[unusedVariable]
            move_player_to_spawn(player);
    }
}

template <>
void Game::handle<GetShopPricesCommand>(const std::string& player_name,
                                        [[maybe_unused]] const GetShopPricesCommand& msg) {
    send_msg(player_name,
             Message(ShopPricesResponse(shop.get_gun_prices(), shop.get_ammo_prices())));
}

template <>
void Game::handle<BuyGunCommand>(const std::string& player_name, const BuyGunCommand& msg) {
    auto& player = state.get_player(player_name);
    if (!state.get_phase().is_buying_phase() || !physics_system.player_in_spawn(player_name) ||
        !shop.can_buy_gun(msg.get_gun(), player->get_inventory())) {
        send_msg(player_name, Message(ErrorResponse()));
        return;
    }

    auto gun = player->drop_primary_weapon();
    if (gun.has_value())
        state.add_dropped_gun(std::move(gun.value()), player->get_hitbox().center);
    shop.buy_gun(msg.get_gun(), player->get_inventory());
}

template <>
void Game::handle<BuyAmmoCommand>(const std::string& player_name, const BuyAmmoCommand& msg) {
    auto& player = state.get_player(player_name);
    if (!state.get_phase().is_buying_phase() || !physics_system.player_in_spawn(player_name) ||
        !shop.can_buy_ammo(msg.get_slot(), player->get_inventory())) {
        send_msg(player_name, Message(ErrorResponse()));
        return;
    }

    shop.buy_ammo(msg.get_slot(), player->get_inventory());
}

template <>
void Game::handle<MoveCommand>(const std::string& player_name, const MoveCommand& msg) {
    if (state.get_phase().is_buying_phase())
        return;

    auto& player = state.get_player(player_name);
    player->handle_start_moving(msg.get_direction());
}

template <>
void Game::handle<StopPlayerCommand>(const std::string& player_name,
                                     [[maybe_unused]] const StopPlayerCommand& msg) {
    auto& player = state.get_player(player_name);
    player->handle_stop_moving();
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
    player->handle_start_attacking();
}

template <>
void Game::handle<SwitchItemCommand>(const std::string& player_name, const SwitchItemCommand& msg) {
    auto& player = state.get_player(player_name);
    player->handle_switch_item(msg.get_slot());
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
    send_msg(player_name, Message(ScoreboardResponse(std::move(scoreboard))));
}

template <>
void Game::handle<StartPlantingBombCommand>(const std::string& player_name,
                                            [[maybe_unused]] const StartPlantingBombCommand& msg) {
    if (!state.get_phase().is_playing_phase() || !physics_system.player_in_bomb_site(player_name))
        return;

    auto& player = state.get_player(player_name);
    player->handle_start_planting(state.get_phase().get_time_now());
}

template <>
void Game::handle<StopPlantingBombCommand>(const std::string& player_name,
                                           [[maybe_unused]] const StopPlantingBombCommand& msg) {
    if (!state.get_phase().is_playing_phase() || !physics_system.player_in_bomb_site(player_name))
        return;

    auto& player = state.get_player(player_name);
    player->handle_stop_planting(state.get_phase().get_time_now());
}

template <>
void Game::handle<StartDefusingBombCommand>(const std::string& player_name,
                                            [[maybe_unused]] const StartDefusingBombCommand& msg) {
    auto& player = state.get_player(player_name);
    if (!state.get_phase().is_bomb_planted_phase() ||
        !physics_system.player_collides_with_bomb(player))
        return;

    auto& bomb = state.get_bomb().value().item;
    player->handle_start_defusing(bomb, state.get_phase().get_time_now());
}

template <>
void Game::handle<StopDefusingBombCommand>(const std::string& player_name,
                                           [[maybe_unused]] const StopDefusingBombCommand& msg) {
    auto& player = state.get_player(player_name);
    if (!state.get_phase().is_bomb_planted_phase() ||
        !physics_system.player_collides_with_bomb(player))
        return;

    auto& bomb = state.get_bomb().value().item;
    player->handle_stop_defusing(bomb, state.get_phase().get_time_now());
}

template <>
void Game::handle<PickUpItemCommand>(const std::string& player_name,
                                     [[maybe_unused]] const PickUpItemCommand& msg) {
    auto& player = state.get_player(player_name);

    if (physics_system.player_collides_with_bomb(player)) {
        player->pick_bomb(std::move(state.remove_bomb()));
        return;
    }

    auto gun_pos = physics_system.get_player_colliding_gun_pos(player);
    if (!gun_pos.has_value())
        return;
    if (player->get_inventory().has_item_in_slot(ItemSlot::Primary)) {
        auto old_gun = player->drop_primary_weapon();
        auto new_gun = state.remove_dropped_gun_at_pos(gun_pos.value());
        if (old_gun.has_value())
            state.add_dropped_gun(std::move(old_gun.value()), player->get_hitbox().center);
        player->pick_gun(std::move(new_gun));
    }
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

float Game::get_tick_duration() {
    TimePoint now = state.get_phase().get_time_now();
    if (last_tick == TimePoint()) {
        return 1.0f / GameConfig::tickrate;
    } else {
        if (now < last_tick)
            last_tick = now;
        if (last_tick == now) {
            return 1.0f / GameConfig::tickrate;
        } else {
            float elapsed = std::chrono::duration<float>(now - last_tick).count();
            return elapsed;
        }
    }
}

void Game::give_bomb_to_random_tt(Bomb&& bomb) {
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
    player->pick_bomb(std::move(bomb));
}

void Game::prepare_new_round() {
    state.advance_round();
    for (const auto& [p_name, player]: state.get_players()) {  // cppcheck-suppress[unusedVariable]
        move_player_to_spawn(player);
        player->reset();
        auto bomb = player->drop_bomb();
        if (bomb.has_value())
            state.add_bomb(std::move(bomb.value()), player->get_hitbox().center);
    }
    if (state.get_bomb().has_value()) {
        state.get_bomb().value().item.reset();
        give_bomb_to_random_tt(std::move(state.remove_bomb()));
    }
    state.get_dropped_guns().clear();
}

void Game::move_player_to_spawn(const std::unique_ptr<Player>& player) {
    if (player->is_tt())
        player->move_to_pos(physics_system.random_spawn_tt_pos());
    else
        player->move_to_pos(physics_system.random_spawn_ct_pos());
}

void Game::broadcast(const Message& msg) {
    for (const auto& [p_name, _]: state.get_players()) send_msg(p_name, msg);
}

void Game::send_msg(const std::string& player_name, const Message& msg) {
    output_messages.emplace_back(player_name, msg);
}

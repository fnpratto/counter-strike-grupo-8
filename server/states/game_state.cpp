#include "game_state.h"

#include <utility>

#include "server/game/game_config.h"
#include "server/items/gun.h"
#include "server/physics/circular_hitbox.h"
#include "server/physics/rect_hitbox.h"

GameState::GameState(std::shared_ptr<Clock>&& game_clock, int max_players,
                     const std::vector<std::pair<GunType, Vector2D>>& guns,
                     GameConfig&& game_config):
        config(std::move(game_config)),
        phase(std::move(game_clock), config.phase_times),
        max_players(max_players) {
    for (const auto& [gun_type, pos]: guns)
        add_dropped_gun(std::move(std::make_unique<Gun>(
                                gun_type, config.items_config.get_gun_config(gun_type))),
                        pos);
    updates = get_full_update();
}

bool GameState::player_is_in_game(const std::string& player_name) const {
    return players.find(player_name) != players.end();
}

bool GameState::all_players_ready() const {
    for (auto& [_, player]: players) {  // cppcheck-suppress[unusedVariable]
        if (!player->is_ready())
            return false;
    }
    return true;
}

bool GameState::game_is_full() const { return static_cast<int>(players.size()) == max_players; }

bool GameState::team_is_full(const Team& team) const {
    int max_team_players = max_players / 2;
    if (team == Team::TT)
        return get_num_tts() == max_team_players;
    return get_num_cts() == max_team_players;
}

bool GameState::is_round_end_condition() const {
    bool bomb_exploded = bomb.has_value() && bomb.value().item.is_exploded();
    bool bomb_defused = bomb.has_value() && bomb.value().item.is_defused();
    return get_num_tts() == 0 || get_num_cts() == 0 || bomb_exploded || bomb_defused;
}

std::map<std::string, ScoreboardEntry> GameState::get_scoreboard() const {
    std::map<std::string, ScoreboardEntry> scoreboard;
    for (const auto& [p_name, player]: get_players())
        scoreboard.emplace(p_name, player->get_scoreboard_entry());
    return scoreboard;
}

int GameState::get_num_rounds() const { return num_rounds; }

int GameState::get_num_tts() const {
    int num_tts = 0;
    for (const auto& [_, player]: players)  // cppcheck-suppress[unusedVariable]
        if (player->is_tt() && !player->is_dead())
            num_tts++;

    return num_tts;
}

int GameState::get_num_cts() const {
    int num_cts = 0;
    for (const auto& [_, player]: players)  // cppcheck-suppress[unusedVariable]
        if (player->is_ct() && !player->is_dead())
            num_cts++;

    return num_cts;
}

const GameConfig& GameState::get_config() const { return config; }

GamePhase& GameState::get_phase() { return phase; }

const std::map<std::string, std::unique_ptr<Player>>& GameState::get_players() const {
    return players;
}

const std::unique_ptr<Player>& GameState::get_player(const std::string& player_name) const {
    auto it = players.find(player_name);
    if (it == players.end())
        throw std::runtime_error("Player not found");
    return it->second;
}

std::vector<WorldItem<std::unique_ptr<Gun>>>& GameState::get_dropped_guns() { return dropped_guns; }

std::optional<WorldItem<Bomb>>& GameState::get_bomb() { return bomb; }

void GameState::advance_round() {
    num_rounds += 1;
    updates.set_num_rounds(num_rounds);
}

void GameState::swap_players_teams() {
    auto characters_tt = get_characters_tt();
    auto characters_ct = get_characters_ct();
    for (auto& [_, player]: players) {  // cppcheck-suppress[unusedVariable]
        if (player->is_tt()) {
            CharacterType actual_character = player->get_character_type();
            auto it = std::find(characters_tt.begin(), characters_tt.end(), actual_character);
            int idx_character = std::distance(characters_tt.begin(), it);
            player->select_team(Team::CT);
            player->select_character(characters_ct[idx_character]);
        } else {
            CharacterType actual_character = player->get_character_type();
            auto it = std::find(characters_ct.begin(), characters_ct.end(), actual_character);
            int idx_character = std::distance(characters_ct.begin(), it);
            player->select_team(Team::TT);
            player->select_character(characters_tt[idx_character]);
        }
    }
}

void GameState::add_player(const std::string& player_name, Team team, const Vector2D& pos) {
    if (players.find(player_name) != players.end())
        throw std::runtime_error("Player already exists");
    CharacterType default_character = get_default_character(team);
    players[player_name] = std::make_unique<Player>(team, default_character,
                                                    CircularHitbox::player_hitbox(pos).get_bounds(),
                                                    config.player_config, config.items_config);
}

void GameState::add_dropped_gun(std::unique_ptr<Gun>&& gun, const Vector2D& pos) {
    Rectangle gun_hitbox = RectHitbox::gun_hitbox(pos).get_bounds();
    updates.set_dropped_guns({WorldItem<GunType>{gun->get_type(), gun_hitbox}});
    dropped_guns.emplace_back(std::move(gun), gun_hitbox);
}

std::unique_ptr<Gun> GameState::remove_dropped_gun_at_pos(const Vector2D& pos) {
    auto it = std::find_if(dropped_guns.begin(), dropped_guns.end(),
                           [&pos](const WorldItem<std::unique_ptr<Gun>>& item) {
                               return item.hitbox.get_pos() == pos;
                           });
    if (it == dropped_guns.end())
        throw std::runtime_error("Dropped gun not found at the specified position");

    std::unique_ptr<Gun> gun = std::move(it->item);
    dropped_guns.erase(it);

    for (auto& dg: dropped_guns)
        updates.set_dropped_guns({WorldItem<GunType>{dg.item->get_type(), dg.hitbox}});

    return gun;
}

void GameState::add_bomb(Bomb&& bomb, const Vector2D& pos) {
    this->bomb = WorldItem<Bomb>{std::move(bomb), RectHitbox::bomb_hitbox(pos).get_bounds()};
}

Bomb GameState::remove_bomb() {
    if (!bomb.has_value())
        throw std::runtime_error("Bomb not found");
    Bomb removed_bomb = std::move(bomb.value().item);
    bomb.reset();
    updates.set_bomb(std::optional<WorldItem<BombUpdate>>());
    return removed_bomb;
}

std::vector<CharacterType> GameState::get_characters_tt() const {
    return {CharacterType::Pheonix, CharacterType::L337_Krew, CharacterType::Arctic_Avenger,
            CharacterType::Guerrilla};
}

std::vector<CharacterType> GameState::get_characters_ct() const {
    return {CharacterType::Seal_Force, CharacterType::German_GSG_9, CharacterType::UK_SAS,
            CharacterType::French_GIGN};
}

CharacterType GameState::get_default_character(Team team) const {
    auto characters = (team == Team::TT) ? get_characters_tt() : get_characters_ct();
    return characters[0];
}

Team GameState::get_winning_team() const {
    if (is_tts_win_condition())
        return Team::TT;
    return Team::CT;
}

bool GameState::is_tts_win_condition() const {
    bool bomb_exploded = bomb.has_value() && bomb.value().item.is_exploded();
    return get_num_cts() == 0 || bomb_exploded;
}

void GameState::give_rewards_to_players(Team winning_team) {
    for (const auto& [p_name, player]: players) {
        if ((winning_team == Team::TT && player->is_tt()) ||
            (winning_team == Team::CT && player->is_ct()))
            player->add_rewards(config.scores.win, config.bonifications.win);
        else
            player->add_rewards(config.scores.loss, config.bonifications.loss);
    }
}

void GameState::clear_updates() {
    State<GameUpdate>::clear_updates();
    phase.clear_updates();
    for (auto& [_, player]: players)  // cppcheck-suppress[unusedVariable]
        player->clear_updates();
    for (auto& dg: dropped_guns) dg.item->clear_updates();
    if (bomb.has_value())
        bomb.value().item.clear_updates();
}

GameUpdate GameState::get_updates() const {
    GameUpdate update = updates;

    update.set_phase(phase.get_updates());

    for (const auto& [name, player]: players)
        update.add_players_change(name, player->get_updates());

    if (bomb.has_value())
        update.set_bomb(
                WorldItem<BombUpdate>{bomb.value().item.get_updates(), bomb.value().hitbox});

    return update;
}

GameUpdate GameState::get_full_update() const {
    GameUpdate update;

    update.set_phase(phase.get_full_update());
    update.set_num_rounds(num_rounds);

    for (const auto& [name, player]: players)
        update.add_players_change(name, player->get_full_update());

    for (const auto& dg: dropped_guns)
        update.set_dropped_guns({WorldItem<GunType>{dg.item->get_type(), dg.hitbox}});

    if (bomb.has_value())
        update.set_bomb(
                WorldItem<BombUpdate>{bomb.value().item.get_full_update(), bomb.value().hitbox});
    else
        update.set_bomb(std::optional<WorldItem<BombUpdate>>());

    return update;
}

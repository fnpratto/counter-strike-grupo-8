#include "game_state.h"

#include <utility>

GameState::GameState(std::shared_ptr<Clock>&& game_clock, int max_players):
        phase(std::move(game_clock)), max_players(max_players) {
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

int GameState::get_num_rounds() const { return num_rounds; }

int GameState::get_num_tts() const {
    int num_tts = 0;
    for (const auto& [_, player]: players)  // cppcheck-suppress[unusedVariable]
        if (player->is_tt())
            num_tts++;

    return num_tts;
}

int GameState::get_num_cts() const {
    int num_cts = 0;
    for (const auto& [_, player]: players)  // cppcheck-suppress[unusedVariable]
        if (player->is_ct())
            num_cts++;

    return num_cts;
}

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

void GameState::advance_round() { set_num_rounds(num_rounds + 1); }

void GameState::set_num_rounds(int rounds) {
    if (num_rounds == rounds)
        return;
    num_rounds = rounds;
    updates.set_num_rounds(rounds);
}

void GameState::swap_players_teams() {
    for (auto& [_, player]: players) {  // cppcheck-suppress[unusedVariable]
        if (player->is_tt()) {
            player->select_team(Team::CT);
        } else if (player->is_ct()) {
            player->select_team(Team::TT);
        }
    }
}

void GameState::add_player(const std::string& player_name, std::unique_ptr<Player> player) {
    if (players.find(player_name) != players.end())
        throw std::runtime_error("Player already exists");
    players[player_name] = std::move(player);
}

void GameState::add_dropped_gun(std::unique_ptr<Gun>&& gun, const Vector2D& pos) {
    updates.set_dropped_guns({std::make_pair(gun->get_type(), pos)});
    dropped_guns.emplace_back(std::move(gun), pos);
}

void GameState::add_bomb(Bomb&& bomb, const Vector2D& pos) {
    this->bomb = std::make_pair(std::move(bomb), pos);
}

void GameState::clear_updates() {
    State<GameUpdate>::clear_updates();
    phase.clear_updates();
    for (auto& [_, player]: players)  // cppcheck-suppress[unusedVariable]
        player->clear_updates();
}

GameUpdate GameState::get_updates() const {
    GameUpdate update = updates;

    update.set_phase(phase.get_updates());
    for (const auto& [name, player]: players)
        update.add_players_change(name, player->get_updates());

    return update;
}

GameUpdate GameState::get_full_update() const {
    GameUpdate update;
    update.set_phase(phase.get_full_update());
    update.set_num_rounds(num_rounds);
    for (const auto& [name, player]: players)
        update.add_players_change(name, player->get_full_update());
    for (const auto& [gun, pos]: dropped_guns)
        update.set_dropped_guns({std::make_pair(gun->get_type(), pos)});
    return update;
}

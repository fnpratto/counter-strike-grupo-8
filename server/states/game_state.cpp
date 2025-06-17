#include "game_state.h"

#include <utility>

#include "server/physics/circular_hitbox.h"
#include "server/physics/rect_hitbox.h"

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

const std::vector<WorldItem<std::unique_ptr<Gun>>>& GameState::get_dropped_guns() const {
    return dropped_guns;
}

const std::optional<WorldItem<Bomb>>& GameState::get_bomb() const { return bomb; }

void GameState::advance_round() {
    num_rounds += 1;
    updates.set_num_rounds(num_rounds);
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

void GameState::add_player(const std::string& player_name, Team team, const Vector2D& pos) {
    if (players.find(player_name) != players.end())
        throw std::runtime_error("Player already exists");
    players[player_name] =
            std::make_unique<Player>(team, CircularHitbox::player_hitbox(pos).get_bounds());
}

void GameState::add_dropped_gun(std::unique_ptr<Gun>&& gun, const Vector2D& pos) {
    Rectangle gun_hitbox = RectHitbox::gun_hitbox(pos).get_bounds();
    updates.set_dropped_guns({WorldItem<GunType>{gun->get_type(), gun_hitbox}});
    dropped_guns.emplace_back(std::move(gun), gun_hitbox);
}

std::unique_ptr<Gun>&& GameState::remove_dropped_gun_at_pos(const Vector2D& pos) {
    auto it = std::find_if(dropped_guns.begin(), dropped_guns.end(),
                           [&pos](const WorldItem<std::unique_ptr<Gun>>& item) {
                               return item.hitbox.get_pos() == pos;
                           });
    if (it == dropped_guns.end())
        throw std::runtime_error("Dropped gun not found at the specified position");

    std::unique_ptr<Gun>&& gun = std::move(it->item);
    dropped_guns.erase(it);

    for (auto& dg: dropped_guns)
        updates.set_dropped_guns({WorldItem<GunType>{dg.item->get_type(), dg.hitbox}});

    return std::move(gun);
}

void GameState::add_bomb(Bomb&& bomb, const Vector2D& pos) {
    this->bomb = WorldItem<Bomb>{std::move(bomb), RectHitbox::bomb_hitbox(pos).get_bounds()};
}

Bomb&& GameState::remove_bomb() {
    if (!bomb.has_value())
        throw std::runtime_error("Bomb not found");
    updates.set_bomb(std::optional<WorldItem<BombUpdate>>());
    return std::move(bomb.value().item);
}

Team GameState::get_winning_team() const {
    if (is_tts_win_condition())
        return Team::TT;
    return Team::CT;
}

// TODO: Add bomb planted win condition
bool GameState::is_tts_win_condition() const { return get_num_cts() == 0; }

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
    return update;
}

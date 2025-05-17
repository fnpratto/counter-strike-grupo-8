#include <vector>

#include "game_state.h"
#include "server/cons.h"
#include "server/errors.h"
#include "server/utils/vector_2d.h"

using namespace GameConfig;
using namespace PlayerInitialConfig;

GameState::GameState(const Clock& clock, const Shop& shop) :
        phase(clock),
        shop(shop) {}

void GameState::add_player(const std::string& player_name) {
    if (!can_join(player_name))
        throw JoinGameError();

    Team team = choose_player_team();
    Player player = Player(team, Inventory(), full_health, set_initial_pos());
    players.emplace(player_name, player);
    if (team == Team::Terrorist) {
        num_tts++;
    } else {
        num_cts++;
    }
}

Vector2D GameState::set_initial_pos() {
    // TODO: calculate init pos according to team spawn
    // TODO: check colissions with other players and objects
    return Vector2D();
}

void GameState::select_team(const std::string& player_name, Team team) {
    if (!player_can_select_team(team))
        throw SelectTeamError();
    
    Team old_team;
    if (players.at(player_name).is_tt()) {
        old_team = Team::Terrorist;
    } else {
        old_team = Team::CounterTerrorist;
    }
    if (old_team == team)
        return;

    players.at(player_name).select_team(team);

    if (team == Team::Terrorist) {
        num_cts--;
        num_tts++;
    } else {
        num_tts--;
        num_cts++;
    }
}

// TODO: revise logic for starting a game.
void GameState::start_game() {
    if (phase.is_started())
        throw StartGameError();
    if (num_tts > 0)
        give_bomb_to_random_tt();
    phase.start_buying_phase();
}

void GameState::buy_gun(const std::string& player_name, GunType gun) {
    int gun_price = shop.get_gun_price(gun);
    players.at(player_name).buy_gun(gun, gun_price);
}

void GameState::buy_ammo(const std::string& player_name, GunType gun) {
    int num_mags = 1;
    if (gun == GunType::M3) {
        num_mags = 8;
    }
    WeaponSlot slot = (gun == GunType::Glock) ? WeaponSlot::Secondary : WeaponSlot::Primary;
    int ammo_price = shop.get_ammo_price(gun, num_mags);
    players.at(player_name).buy_ammo(slot, ammo_price, num_mags);
}

void GameState::move(const std::string& player_name, int dx, int dy) {
    Vector2D dir(dx, dy);
    float tick_duration = 1 / tickrate;
    Vector2D step = dir.normalize() * player_speed * tick_duration;
    // TODO: Check collisions
    players.at(player_name).move(step);
}

void GameState::update_round_phase() {
    if (num_rounds == max_rounds / 2)
        swap_teams();
    phase.update();
    if (phase.is_round_finished())
        num_rounds++;
}

Team GameState::choose_player_team() {
    if (num_tts > num_cts)
        return Team::CounterTerrorist;
    return Team::Terrorist;
}

void GameState::give_bomb_to_random_tt() {
    std::vector<std::string> tt_names;

    for (auto& [player_name, player] : players) {
        if (player.is_tt()) {
            tt_names.push_back(player_name);
        }
    }

    int random_index = rand() % num_tts;
    players.at(tt_names[random_index]).pick_bomb();
}

void GameState::swap_teams() {
    for (auto& [_, player] : players) {
        if (player.is_tt()) {
            player.select_team(Team::CounterTerrorist);
        } else if (player.is_ct()) {
            player.select_team(Team::Terrorist);
        }
    }
}

GameState::~GameState() {}

bool GameState::can_join(const std::string& player_name) const {
    return players.find(player_name) == players.end() && !is_full() && !phase.is_started();
}

bool GameState::player_can_select_team(Team& team) const {
    return !team_is_full(team) && !phase.is_started();
}

bool GameState::is_full() const { 
    return static_cast<int>(players.size()) == max_players; 
}

bool GameState::team_is_full(Team& team) const {
    if (team == Team::Terrorist)
        return num_tts == max_team_players;
    return num_cts == max_team_players;
}

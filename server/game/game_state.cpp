#include <vector>

#include "game_state.h"
#include "server/errors.h"
#include <iostream>

GameState::GameState(const GameConfig& conf, const Clock& clock) :
        conf(conf),
        phase(clock, conf.buying_phase_secs, 
                     conf.playing_phase_secs, 
                     conf.round_finished_phase_secs) {}

GameConfig GameState::get_config() { return conf; }

PhaseType GameState::get_phase() { return phase.get_type(); }

void GameState::add_player(const std::string& player_name) {
    if (!can_join(player_name))
        throw JoinGameError();

    Team team = choose_player_team();
    conf.players.emplace(player_name, Player(team, conf.default_inventory, conf.full_health));
    if (team == Team::Terrorist) {
        conf.num_tts++;
    } else {
        conf.num_cts++;
    }
}

void GameState::select_team(const std::string& player_name, Team team) {
    if (!player_can_select_team(team))
        throw SelectTeamError();
    
    Team old_team;
    if (conf.players.at(player_name).is_tt()) {
        old_team = Team::Terrorist;
    } else {
        old_team = Team::CounterTerrorist;
    }
    if (old_team == team)
        return;

    conf.players.at(player_name).select_team(team);

    if (team == Team::Terrorist) {
        conf.num_cts--;
        conf.num_tts++;
    } else {
        conf.num_tts--;
        conf.num_cts++;
    }
}

// TODO: revise logic for starting a game.
void GameState::start_game() {
    if (phase.is_started())
        throw StartGameError();
    if (conf.num_tts > 0)
        give_bomb_to_random_tt();
    phase.start_buying_phase();
}

void GameState::buy_weapon(const std::string& player_name, WeaponType weapon, int weapon_price) {
    int player_money = conf.players.at(player_name).get_inventory().money;
    if (weapon_price > player_money)
        throw BuyWeaponError();
    conf.players.at(player_name).buy_weapon(weapon, weapon_price);
}

void GameState::update_round_phase() {
    if (conf.num_rounds == conf.max_rounds / 2)
        swap_teams();
    phase.update();
    if (phase.get_type() == PhaseType::RoundFinished)
        conf.num_rounds++;
}

Team GameState::choose_player_team() {
    if (conf.num_tts > conf.num_cts)
        return Team::CounterTerrorist;
    return Team::Terrorist;
}

void GameState::give_bomb_to_random_tt() {
    std::vector<std::string> tt_names;

    for (auto& [player_name, player] : conf.players) {
        if (player.is_tt()) {
            tt_names.push_back(player_name);
        }
    }

    int random_index = rand() % conf.num_tts;
    conf.players.at(tt_names[random_index]).pick_bomb(WeaponType::C4);
}

void GameState::swap_teams() {
    for (auto& [_, player] : conf.players) {
        if (player.is_tt()) {
            player.select_team(Team::CounterTerrorist);
        } else if (player.is_ct()) {
            player.select_team(Team::Terrorist);
        }
    }
}

GameState::~GameState() {}

bool GameState::can_join(const std::string& player_name) const {
    return conf.players.find(player_name) == conf.players.end() && !is_full() && !phase.is_started();
}

bool GameState::player_can_select_team(Team& team) const {
    return !team_is_full(team) && !phase.is_started();
}

bool GameState::is_full() const { return conf.players.size() == conf.max_players; }

bool GameState::team_is_full(Team& team) const {
    if (team == Team::Terrorist)
        return conf.num_tts == conf.max_players_per_team;
    return conf.num_cts == conf.max_players_per_team;
}

#include <vector>

#include "game_state.h"
#include "server/errors.h"
#include "server/utils/vector_2d.h"

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
    Player player = Player(team, conf.init_invent, conf.full_health, set_init_pos());
    conf.players.emplace(player_name, player);
    if (team == Team::Terrorist) {
        conf.num_tts++;
    } else {
        conf.num_cts++;
    }
}

Vector2D GameState::set_init_pos() {
    // TODO: calculate init pos according to team spawn
    // TODO: check colissions with other players and objects
    return Vector2D();
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

void GameState::buy_gun(const std::string& player_name, GunType gun, int gun_price) {
    conf.players.at(player_name).buy_gun(gun, gun_price);
}

void GameState::buy_ammo(const std::string& player_name, GunType gun, int mag_price) {
    int num_mags = 1;
    if (gun == GunType::M3) {
        num_mags = 8;
    }
    WeaponSlot slot = gun == GunType::Glock ? WeaponSlot::Secondary : WeaponSlot::Primary;
    conf.players.at(player_name).buy_ammo(slot, mag_price, num_mags);
}

void GameState::move(const std::string& player_name, int dx, int dy) {
    Vector2D dir(dx, dy);
    float tick_duration = 1 / conf.tickrate;
    Vector2D step = dir.normalize() * conf.player_speed * tick_duration;

    // TODO: Check collisions

    conf.players.at(player_name).move(step);
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
    conf.players.at(tt_names[random_index]).pick_bomb();
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

bool GameState::is_full() const { 
    return static_cast<int>(conf.players.size()) == conf.max_players; 
}

bool GameState::team_is_full(Team& team) const {
    if (team == Team::Terrorist)
        return conf.num_tts == conf.max_team_players;
    return conf.num_cts == conf.max_team_players;
}

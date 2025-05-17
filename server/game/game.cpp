#include "game.h"

#include <string>

#include "server/errors.h"

Game::Game(const std::string& name, const Clock& clock, const Shop& shop): 
        name(name),
        state(clock, shop) {}

void Game::tick(Message msg, const std::string& player_name) {
    chk_valid_player_name_or_fail(player_name);
    handle_msg(msg, player_name);
    state.update_round_phase();
}

void Game::handle_msg(Message msg, const std::string& player_name) {
    MessageType msg_type = msg.get_type();
    if (msg_type == MessageType::JOIN_GAME_CMD) {
        state.add_player(player_name);
    } else if (msg_type == MessageType::SELECT_TEAM_CMD) {
        Team team = msg.get_content<SelectTeamCommand>().get_team();
        state.select_team(player_name, team);
    } else if (msg_type == MessageType::START_GAME_CMD) {
        state.start_game();
    } else if (msg_type == MessageType::BUY_GUN_CMD) {
        GunType gun = msg.get_content<BuyGunCommand>().get_gun();
        state.buy_gun(player_name, gun);
    } else if (msg_type == MessageType::BUY_AMMO_CMD) {
        GunType gun = msg.get_content<BuyAmmoCommand>().get_gun();
        state.buy_ammo(player_name, gun);
    } else if (msg_type == MessageType::MOVE_CMD) {
        int dx = msg.get_content<MoveCommand>().get_dx();
        int dy = msg.get_content<MoveCommand>().get_dy();
        state.move(player_name, dx, dy);
    }
}

Game::~Game() {}

void Game::chk_valid_player_name_or_fail(const std::string& player_name) {
    if (player_name.empty())
        throw InvalidPlayerNameError();
}

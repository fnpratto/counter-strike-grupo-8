#pragma once

#include <memory>

#include "game_state.h"
#include "game_logic.h"

class GameStateBuilder {
private:
    static PhaseType build_game_phase(const GamePhase& phase) {
        if (!phase.is_started()) {
            return PhaseType::NotStarted;
        } else if (phase.is_buying_phase()) {
            return PhaseType::Buying;
        } else if (phase.is_round_finished()) {
            return PhaseType::RoundFinished;
        }
        return PhaseType::Playing;
    }

    static GunState build_gun_state(std::unique_ptr<Gun> gun) {
        GunState gun_state;
        gun_state.gun = gun->get_type();
        gun_state.bullets_per_mag = gun->get_bullets_per_mag();
        gun_state.mag_ammo = gun->get_mag_ammo();
        gun_state.reserve_ammo = gun->get_reserve_ammo();
        return gun_state;
    }

    static InventoryState build_inv_state(const Player& player) {
        Inventory inv = player.get_inventory();
        InventoryState inv_state;
        inv_state.money = inv.get_money();

        if (player.has_prim_weapon())
            inv_state.guns[WeaponSlot::Primary] = build_gun_state(inv.get_prim_weapon());
        inv_state.guns[WeaponSlot::Secondary] = build_gun_state(inv.get_sec_weapon());

        if (player.has_bomb())
            inv_state.utilities[WeaponSlot::Bomb] = inv.get_bomb()->get_type();
        inv_state.utilities[WeaponSlot::Melee] = inv.get_melee_weapon()->get_type();
        return inv_state;
    }

    static PlayerState build_player_state(const Player& player) {
        PlayerState player_state;
        if (player.is_tt()) {
            player_state.team = Team::Terrorist;
        } else if (player.is_ct()) {
            player_state.team = Team::CounterTerrorist;
        }

        player_state.inventory = build_inv_state(player);

        player_state.health = player.get_health();
        player_state.pos_x = player.get_pos_x();
        player_state.pos_y = player.get_pos_y();
        player_state.current_weapon = player.get_current_weapon();
        return player_state;
    }

public:
    static GameState build(const GameLogic& game_logic) {
        GameState game_state;

        game_state.phase = build_game_phase(game_logic.phase);
        game_state.num_rounds = game_logic.num_rounds;
        game_state.num_tts = game_logic.num_tts;
        game_state.num_cts = game_logic.num_cts;

        for (auto& [player_name, player] : game_logic.players) {
            game_state.players[player_name] = build_player_state(player);
        }

        return game_state;
    }
};

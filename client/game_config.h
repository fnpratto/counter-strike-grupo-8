#pragma once

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "common/models.h"

namespace GameConfig {

struct GunInfo {
    std::string id;
    std::string name;
    std::string price;
};

struct AmmoInfo {
    std::string id;
    std::string name;
    std::string price;
};
// --- Paths ---
namespace Paths {
constexpr std::string_view GunsInventory = "../assets/gfx/hud/guns_inventory.xcf";
constexpr std::string_view HudSlotClicked = "../assets/gfx/shop/hud_slot_clicked.xcf";
constexpr std::string_view HudSlotBlocked = "../assets/gfx/shop/hud_slot_blocked.xcf";
constexpr std::string_view Font = "../assets/gfx/fonts/joystix_monospace.otf";
constexpr std::string_view Ammo = "../assets/gfx/guns/ammo.xcf";
constexpr std::string_view Ammo1 = "../assets/gfx/guns/ammo1.xcf";
constexpr std::string_view BACKGROUND_PATH = "../assets/gfx/backgrounds/water1.jpg";
constexpr std::string_view POINTER_PATH = "../assets/gfx/hud/pointer.xcf";
constexpr std::string_view MONEY_PATH = "../assets/gfx/hud/hud_symbols.xcf";
constexpr std::string_view LIFE_PATH = "../assets/gfx/hud/hud_symbols.xcf";
constexpr std::string_view FONT_PATH = "../assets/gfx/fonts/joystix_monospace.otf";
constexpr std::string_view BULLET_PATH = "../assets/gfx/hud/bullet-icon1.xcf";
constexpr std::string_view TRAPECIO_PATH = "../assets/gfx/hud/trapezoid.xcf";
constexpr std::string_view HUD_NUMS_PATH = "../assets/gfx/fonts/hud_nums.xcf";
constexpr std::string_view PARALELO_RED_PATH = "../assets/gfx/hud/parallelogram.xcf";
constexpr std::string_view PARALELO_BLUE_PATH = "../assets/gfx/hud/parallelogram_blue.xcf";
constexpr std::string_view PARALELO_RED_O_PATH = "../assets/gfx/hud/parallelogram_red_op.xcf";
constexpr std::string_view PARALELO_BLUE_O_PATH = "../assets/gfx/hud/parallelogram_blue_op.xcf";
constexpr std::string_view HUD_NUMS_XCF = "../assets/gfx/fonts/hud_nums.xcf";
constexpr std::string_view MUTE_ICON_PATH = "../assets/gfx/hud/hud_voice.xcf";
constexpr std::string_view GUNS_INVENTORY_PATH = "../assets/gfx/hud/guns_inventory.xcf";
constexpr std::string_view WHITE_LINE_PATH = "../assets/gfx/hud/linea_blanca.xcf";

constexpr std::string_view GUNS_PATH = "../assets/gfx/guns/guns.xcf";
constexpr std::string_view HUD_SLOT_PATH = "../assets/gfx/shop/hud_slot.xcf";
constexpr std::string_view HUD_SLOT_CLICKED_PATH = "../assets/gfx/shop/hud_slot_clicked.xcf";
constexpr std::string_view HUD_SLOT_BLOCKED_PATH = "../assets/gfx/shop/hud_slot_blocked.xcf";
constexpr std::string_view FONT_PAT = "../assets/gfx/fonts/joystix_monospace.otf";
constexpr std::string_view AMMO_PATH = "../assets/gfx/guns/ammo.xcf";
constexpr std::string_view BACKGROUND_WINNER_PATH = "../assets/gfx/hud/end_round.xcf";


/// Music

constexpr std::string_view MENU_MUSIC_PATH = "../assets/sfx/menu.wav";
constexpr std::string_view GAME_MUSIC_PATH = "../assets/sfx/env/ambience1.wav";
constexpr std::string_view IN_GAME_MUSIC_PATH = "../assets/sfx/env/demonwind.ogg";
constexpr std::string_view PLANTED_GAME_MUSIC_PATH = "../assets/sfx/env/siren.wav";

// Sounds
constexpr std::string_view CT_WIN_SOUND_PATH = "../assets/sfx/radio/ctwin.ogg";
constexpr std::string_view TT_WIN_SOUND_PATH = "../assets/sfx/radio/terwin.ogg";
constexpr std::string_view ITEM_PICK_SOUND_PATH = "../assets/sfx/items/pickup.wav";
constexpr std::string_view RECHARGE_SOUND_PATH = "../assets/sfx/items/ammo.wav";
constexpr std::string_view HIT_SOUND_PATH = "../assets/sfx/player/hit1.wav";
constexpr std::string_view BOMB_PLANTED_SOUND_PATH = "../assets/sfx/radio/bombpl.ogg";
constexpr std::string_view BOMB_DEFUSED_SOUND_PATH = "../assets/sfx/radio/bombdef.ogg";
constexpr std::string_view SWITCH_TEAMS_SOUND_PATH = "../assets/sfx/radio/regroup.ogg";
constexpr std::string_view ERROR_SOUND_PATH = "../assets/sfx/wpn_denyselect.wav";
constexpr std::string_view BOMB_EXPLODED_SOUND_PATH = "../assets/sfx/weapons/explode2.wav";

constexpr std::string_view AWP_ITEM_PATH = "../assets/gfx/map/awp_d.bmp";
constexpr std::string_view M3_ITEM_PATH = "../assets/gfx/map/m3_d.bmp";
constexpr std::string_view AK_ITEM_PATH = "../assets/gfx/map/ak47_d.bmp";
constexpr std::string_view BOMB_ITEM_PATH = "../assets/gfx/guns/bomb.bmp";
constexpr std::string_view BOMB_PLANTED_ITEM_PATH = "../assets/gfx/map/bomb_d.bmp";
}  // namespace Paths

// --- Gun Type Offsets ---

inline std::map<std::string, int> offsetInventory = {{"awp", 0},   {"m3", 1},    {"ak4", 2},
                                                     {"glock", 3}, {"knife", 4}, {"bomb", 5}};

// --- Gun Type Mapping ---
inline const std::map<int, GunType> GunIndexMap = {
        {0, GunType::AK47}, {1, GunType::M3}, {2, GunType::AWP}};

inline const std::map<int, GunType> AmmoIndexMap = {
        {3, GunType::AK47}, {4, GunType::M3}, {5, GunType::AWP}, {6, GunType::Glock}};

// --- GunType to string ---
constexpr std::string_view GunTypeToStr(GunType type) {
    switch (type) {
        case GunType::AK47:
            return "ak4";
        case GunType::M3:
            return "m3";
        case GunType::AWP:
            return "awp";
        case GunType::Glock:
            return "glock";
        default:
            return "";
    }
}
}  // namespace GameConfig

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

constexpr std::string_view GUNS_PATH = "../assets/gfx/guns/guns.xcf";
constexpr std::string_view HUD_SLOT_PATH = "../assets/gfx/shop/hud_slot.xcf";
constexpr std::string_view HUD_SLOT_CLICKED_PATH = "../assets/gfx/shop/hud_slot_clicked.xcf";
constexpr std::string_view HUD_SLOT_BLOCKED_PATH = "../assets/gfx/shop/hud_slot_blocked.xcf";
constexpr std::string_view FONT_PAT = "../assets/gfx/fonts/joystix_monospace.otf";
constexpr std::string_view AMMO_PATH = "../assets/gfx/guns/ammo.xcf";

}  // namespace Paths

// --- Gun Type Offsets ---


std::map<std::string, int> offsetInventory = {{"awp", 0},   {"m3", 1},    {"ak4", 2},
                                              {"glock", 3}, {"knife", 4}, {"bomb", 5}};

// --- Gun Type Mapping ---
inline const std::map<int, GunType> GunIndexMap = {
        {0, GunType::AK47}, {1, GunType::M3}, {2, GunType::AWP}};

inline const std::map<int, GunType> AmmoIndexMap = {
        {4, GunType::AK47}, {5, GunType::M3}, {6, GunType::AWP}, {7, GunType::Glock}};

// --- Gun Info ---
inline const std::vector<GunInfo> Guns = {{"1", "", "1000"}, {"2", "", "1500"}, {"3", "", "20"},
                                          {"4", "", ""},     {"5", "", "2000"}, {"6", "", "10"},
                                          {"7", "", "30"},   {"8", "", ""}};

inline const std::vector<AmmoInfo> Ammos = {
        {"4", "", "200"}, {"5", "", "300"}, {"6", "", "400"}, {"7", "", "500"}, {"8", "", ""}};

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

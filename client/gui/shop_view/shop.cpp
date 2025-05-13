#include "shop.h"

#include <iostream>

const int VALID_SLOTS = 3;
// Paths
const std::string& GUNS = "../assets/gfx/guns/guns.xcf";
const std::string& HUD_SLOT_PATH = "../assets/gfx/shop/hud_slot.xcf";
const std::string& HUD_SLOT_CLICKED_PATH = "../assets/gfx/shop/hud_slot_clicked.xcf";
const std::string& HUD_SLOT_BLOCKED_PATH = "../assets/gfx/shop/hud_slot_blocked.xcf";
const std::string& FONT_PAT = "../assets/gfx/fonts/joystix_monospace.otf";
const std::string& GUNS_PATH = "../assets/gfx/guns/";
const std::string& GUNS_PATH_AK = "../assets/gfx/guns/ak47_m.xcf";
const std::string& GUNS_PATH_AUG = "../assets/gfx/guns/aug_m.xcf";
const std::string& GUNS_PATH_M3 = "../assets/gfx/guns/m3_m.xcf";
const std::string& GUNS_PATH_AWP = "../assets/gfx/guns/awp_m.xcf";
const std::string& GUNS_PATH_DEAGLE = "../assets/gfx/guns/deagle_m.xcf";
const std::string& GUNS_PATH_FAMAS = "../assets/gfx/guns/famas_m.xcf";
const std::string& GUNS_PATH_FIVESEVEN = "../assets/gfx/guns/fiveseven_m.xcf";
const std::string& GUNS_PATH_G3SG1 = "../assets/gfx/guns/g3sg1_m.xcf";
const int size_guns_w = 150;
const int size_guns_h = 40;
const int size_slots_w = size_guns_w + 80;
const int size_slots_h = size_guns_h + 10;

struct GunInfo {
    std::string name;
    std::string path;
    std::string price;
};


std::vector<GunInfo> guns = {{"1", GUNS_PATH_DEAGLE, "1000"}, {"2", GUNS_PATH_G3SG1, "2000"},
                             {"3", GUNS_PATH_FAMAS, "1500"},  {"4", GUNS_PATH_AUG, ""},
                             {"5", GUNS_PATH_M3, ""},         {"6", GUNS_PATH_AK, ""},
                             {"7", GUNS_PATH_FIVESEVEN, ""},  {"8", GUNS_PATH_AWP, ""}};


shopDisplay::shopDisplay(SdlWindow& window):
        window(window),
        DISPLAY_WIDTH(window.getWidth() / 2),
        DISPLAY_HEIGHT(window.getHeight() / 2),
        back(HUD_SLOT_PATH, window),
        back_chosen(HUD_SLOT_CLICKED_PATH, window),
        gunNumber(FONT_PAT, 20, {255, 255, 255, 255}, window),
        cost_money(FONT_PAT, 20, {255, 255, 255, 255}, window) {

    gun_buy = -1;
    for (const auto& gun: guns) {
        try {
            gun_icons.emplace_back(gun.path, window);
            std::cerr << "Loaded: " << gun.name << " (" << gun.path << ")\n";
        } catch (const std::exception& e) {
            std::cerr << "Failed: " << gun.name << " - " << e.what() << "\n";
        }
    }
}

void shopDisplay::render() {
    renderSlots();
    renderItem();
}

void shopDisplay::renderSlots() {
    int base_x = DISPLAY_WIDTH / 2 - size_slots_w / 2;
    int base_y = DISPLAY_HEIGHT / 2 - size_slots_h / 2;
    for (int i = 0; i < guns.size() / 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            Area src(0, 0, size_guns_w, size_guns_h);
            Area dest(base_x + j * (size_guns_w * 2), base_y + i * (size_guns_h * 2),
                      size_guns_w * 2, size_guns_h * 2);
            if (gun_buy == i * 2 + j) {
                back_chosen.render(src, dest);
            } else {
                back.render(src, dest);
            }
        }
    }
}

void shopDisplay::renderItem() {

    for (size_t i = 0; i < guns.size(); ++i) {
        int row = i / 2;
        int col = i % 2;

        int x = DISPLAY_WIDTH / 2 - size_slots_w / 2 + 10 + col * (size_guns_w * 2 + 10);
        int y = DISPLAY_HEIGHT / 2 - size_slots_h / 2 + 20 + row * (size_guns_h * 2);

        Area src(0, 0, 67, 17);
        Area iconDest(x + 30, y, size_guns_w - 30, size_guns_h + 10);
        int image = i;
        if (image < 4) {
            image = i + 4;
        }
        gun_icons[image].render(src, iconDest);

        // Render gun number
        gunNumber.setTextString(guns[i].name);
        Area numDest(x, y - 10, 15, 15);
        gunNumber.render(numDest);

        // Render price
        if (guns[i].price.empty()) {
            cost_money.setTextString("Not Available");
        } else {
            cost_money.setTextString("$" + guns[i].price);
        }
        Area priceDest(x + size_guns_w + 10, y, 100, 40);
        cost_money.render(priceDest);
    }
}

void shopDisplay::updatePointerPosition(int x, int y) {
    if (x >= DISPLAY_WIDTH / 2 - size_slots_w / 2 && x <= DISPLAY_WIDTH / 2 + size_slots_w / 2 &&
        y >= DISPLAY_HEIGHT / 2 - size_slots_h / 2 && y <= DISPLAY_HEIGHT / 2 + size_slots_h / 2) {
        std::cerr << "Mouse is inside the shop display area." << std::endl;
    } else {
        std::cerr << "Mouse is outside the shop display area." << std::endl;
    }

    int base_x = DISPLAY_WIDTH / 2 - size_slots_w / 2;
    int base_y = DISPLAY_HEIGHT / 2 - size_slots_h / 2;

    int slot_width = size_guns_w * 2;
    int slot_height = size_guns_h * 2;

    int column = (x - base_x) / slot_width;
    int row = (y - base_y) / slot_height;

    if (x >= base_x && x < base_x + slot_width * 2 && y >= base_y &&
        y < base_y + slot_height * (guns.size() / 2)) {
        int slot_index = row * 2 + column;
        if (slot_index >= 0 && slot_index < VALID_SLOTS) {
            std::cerr << "Mouse is over slot: " << slot_index + 1
                      << " (Gun: " << guns[slot_index].name << ")" << std::endl;
            gun_buy = slot_index;
        } else {
            std::cerr << "Mouse is over an invalid slot." << std::endl;
        }
    } else {
        std::cerr << "Mouse is outside the shop display area." << std::endl;
    }
}

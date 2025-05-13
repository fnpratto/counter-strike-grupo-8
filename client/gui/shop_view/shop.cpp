#include "shop.h"

#include <iostream>

const int AMOUNT_GUNS = 8;
// Paths
const std::string& HUD_SLOT_PATH = "../assets/gfx/shop/hud_slot.xcf";
const std::string& FONT_PAT = "../assets/gfx/fonts/joystix_monospace.otf";
const std::string& GUNS_PATH = "../assets/gfx/guns/";
const std::string& GUNS_PATH_AK = "../assets/gfx/guns/ak47_m.xcf";
const std::string& GUNS_PATH_AUG = "../assets/gfx/guns/aug_m.xcf";
const std::string& GUNS_PATH_ELITE = "../assets/gfx/guns/elite_m.xcf";
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
    int price;
};

std::vector<GunInfo> guns = {{"1", GUNS_PATH_ELITE, 1000},     {"2", GUNS_PATH_AWP, 2000},
                             {"3", GUNS_PATH_AK, 1500},        {"4", GUNS_PATH_AUG, 1700},
                             {"5", GUNS_PATH_DEAGLE, 1200},    {"6", GUNS_PATH_FAMAS, 1400},
                             {"7", GUNS_PATH_FIVESEVEN, 1100}, {"8", GUNS_PATH_G3SG1, 1800}};

shopDisplay::shopDisplay(SdlWindow& window):
        window(window),
        DISPLAY_WIDTH(window.getWidth() / 2),
        DISPLAY_HEIGHT(window.getHeight() / 2),
        back(HUD_SLOT_PATH, window),
        gunNumber(FONT_PAT, 20, {255, 255, 255, 255}, window),
        cost_money(FONT_PAT, 20, {255, 255, 255, 255}, window) {


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
            back.render(src, dest);
        }
    }
}
void shopDisplay::renderItem() {

    int start_x = DISPLAY_WIDTH / 2 - size_slots_w / 2 + 10;
    int start_y = DISPLAY_HEIGHT / 2 - size_slots_h / 2 + 20;

    for (size_t i = 0; i < guns.size(); ++i) {

        Area src(0, 0, 67, 17);
        Area iconDest(start_x + 30, start_y, size_guns_w - 30, size_guns_h + 10);
        gun_icons[i].render(src, iconDest);

        // Render gun number
        gunNumber.setTextString(guns[i].name);
        Area numDest(start_x, start_y - 10, 15, 15);
        gunNumber.render(numDest);

        // Render price
        cost_money.setTextString("$" + std::to_string(guns[i].price));
        Area priceDest(start_x + size_guns_w + 10, start_y, 100, 40);
        cost_money.render(priceDest);

        if (i % 2 == 1) {
            start_x = DISPLAY_WIDTH / 2 - size_slots_w / 2 + 10;
            start_y += size_guns_h * 2;
        } else {
            start_x += size_guns_w * 2 + 10;
        }
    }
}

void shopDisplay::updatePointerPosition(int x, int y) {
    std::cerr << "Mouse moved in shop to position (" << x << ", " << y << ")" << std::endl;
}

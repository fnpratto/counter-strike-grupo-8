#include "shop.h"

#include <iostream>

const int VALID_SLOTS = 6;

const std::string& GUNS_PATH = "../assets/gfx/guns/guns.xcf";
const std::string& HUD_SLOT_PATH = "../assets/gfx/shop/hud_slot.xcf";
const std::string& HUD_SLOT_CLICKED_PATH = "../assets/gfx/shop/hud_slot_clicked.xcf";
const std::string& HUD_SLOT_BLOCKED_PATH = "../assets/gfx/shop/hud_slot_blocked.xcf";
const std::string& FONT_PAT = "../assets/gfx/fonts/joystix_monospace.otf";
const std::string& AMMO_PATH = "../assets/gfx/guns/AMMO.xcf";
const std::string& GUNS_PATH_AMMO1 = "../assets/gfx/guns/ammo.xcf";
const std::string& GUNS_PATH_AMMO2 = "../assets/gfx/guns/ammo1.xcf";

struct GunInfo {
    std::string name;
    std::string name_2;
    std::string price;
};


// std::vector<int> render_order = {0, 2, 1, 5, 4, 6, 3, 7};

std::vector<GunInfo> guns = {{"1", "", "1000"},  // gun at (0,0)
                             {"2", "", "1500"},  // gun at (64,0)
                             {"3", "", "20"},    // gun at (128,0)
                             {"4", "", ""},     {"5", "", "2000"}, {"6", "", "10"},
                             {"7", "", "30"},   {"8", "", ""}};

shopDisplay::shopDisplay(SdlWindow& window):
        window(window),
        DISPLAY_WIDTH(window.getWidth()),
        DISPLAY_HEIGHT(window.getHeight()),
        back(HUD_SLOT_PATH, window),
        back_chosen(HUD_SLOT_CLICKED_PATH, window),
        gunNumber(FONT_PAT, 20, {255, 255, 255, 255}, window),
        cost_money(FONT_PAT, 20, {255, 255, 255, 255}, window),
        gun_icons(GUNS_PATH, window),
        ammo_icons(AMMO_PATH, window) {

    float BASE_WIDTH = 800.0f;
    float BASE_HEIGHT = 600.0f;
    float scale_w = DISPLAY_WIDTH / BASE_WIDTH;
    float scale_h = DISPLAY_HEIGHT / BASE_HEIGHT;


    size_guns_w = 150 * scale_w;
    size_guns_h = 40 * scale_h;
    size_slots_w = (scale_w == 1) ? size_guns_w * scale_w : size_guns_w * scale_w / 2;
    size_slots_h = size_guns_h * scale_h;


    gun_buy = -1;
}

void shopDisplay::render() {
    renderSlots();
    renderItem();
}

void shopDisplay::renderSlots() {
    int base_x = DISPLAY_WIDTH / 2 - size_slots_w * 2;
    int base_y = DISPLAY_HEIGHT / 2 - size_slots_h * 2;
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
    // Custom render order (flat)
    std::vector<int> render_order = {0, 2, 1, 5, 4, 6, 3, 7};

    for (size_t idx = 0; idx < render_order.size(); ++idx) {
        int i = render_order[idx];

        int row = idx / 2;
        int col = idx % 2;

        int x = DISPLAY_WIDTH / 2 - size_slots_w * 2 + 10 + col * (size_guns_w * 2 + 10);
        int y = DISPLAY_HEIGHT / 2 - size_slots_h * 2 + 20 + row * (size_guns_h * 2);

        // Get correct sprite from 4x2 grid
        int sprite_col = i % 4;
        int sprite_row = i / 4;
        Area src(sprite_col * 66, sprite_row * 17, 66, 17);

        Area iconDest(x + 30, y, size_guns_w - 30, size_guns_h + 10);
        gun_icons.render(src, iconDest);

        gunNumber.setTextString(guns[i].name);
        Area numDest(x, y - 10, 15, 15);
        gunNumber.render(numDest);

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

    for (int i = 0; i < guns.size() / 2; ++i) {
        if (x >= DISPLAY_WIDTH / 2 - size_slots_w * 2 &&
            x <= DISPLAY_WIDTH / 2 + size_slots_w * 2 &&
            y >= DISPLAY_HEIGHT / 2 - size_slots_h * 2 &&
            y <= DISPLAY_HEIGHT / 2 + size_slots_h * 2) {
            std::cerr << "Mouse is inside the shop display area." << std::endl;
        } else {
            std::cerr << "Mouse is outside the shop display area." << std::endl;
        }

        int base_x = DISPLAY_WIDTH / 2 - size_slots_w * 2;
        int base_y = DISPLAY_HEIGHT / 2 - size_slots_h * 2;

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
                return;
            } else {
                std::cerr << "Mouse is over an invalid slot." << std::endl;
                return;
            }
        } else {
            std::cerr << "Mouse is outside the shop display area." << std::endl;
            return;
        }
    }
}

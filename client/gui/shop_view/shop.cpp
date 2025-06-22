#include "shop.h"

#include <iostream>
#include <map>
#include <optional>

#include "../../game_config.h"
#include "common/commands.h"
#include "common/message.h"
#include "common/models.h"

const int VALID_SLOTS = 6;

shopDisplay::shopDisplay(SdlWindow& window, const GameUpdate& state):
        window(window),
        game_state(state),
        DISPLAY_WIDTH(window.getWidth()),
        DISPLAY_HEIGHT(window.getHeight()),
        gun_icons(std::string(GameConfig::Paths::GUNS_PATH), window),
        ammo_icons(std::string(GameConfig::Paths::AMMO_PATH), window),
        cost_money(std::string(GameConfig::Paths::FONT_PAT), 20, {255, 255, 255, 255}, window),
        back(std::string(GameConfig::Paths::HUD_SLOT_PATH), window),
        back_chosen(std::string(GameConfig::Paths::HUD_SLOT_CLICKED_PATH), window),
        gunNumber(std::string(GameConfig::Paths::FONT_PAT), 20, {255, 255, 255, 255}, window),
        gun_buy(-1),
        guns({{"1", "", "1000"}, {"2", "", "1500"}, {"3", "", "20"}}),
        ammo({{"4", "", "200"},
              {"5", "", "300"},
              {"6", "", "400"},
              {"7", "", "500"},
              {"8", "", ""}}) {

    float BASE_WIDTH = 800.0f;
    float BASE_HEIGHT = 600.0f;
    float scale_w = DISPLAY_WIDTH / BASE_WIDTH;
    float scale_h = DISPLAY_HEIGHT / BASE_HEIGHT;

    size_guns_w = 150 * scale_w;
    size_guns_h = 40 * scale_h;
    size_slots_w = (scale_w == 1) ? size_guns_w * scale_w : size_guns_w * scale_w / 2;
    size_slots_h = size_guns_h * scale_h;

    gun_buy = -1;
    active = false;
}
void shopDisplay::updateShopState(bool state) { active = state; }


void shopDisplay::updatePrices(const ShopPricesResponse& response) {
    auto gun_prices = response.get_gun_prices();
    for (const auto& [gun_type, price]: gun_prices) {
        // match gun_type to your guns[i] index
        switch (gun_type) {
            case GunType::AK47:
                guns[0].price = std::to_string(price);
                guns[0].id = "1";
                break;
            case GunType::M3:
                guns[1].price = std::to_string(price);
                guns[1].id = "2";
                break;
            case GunType::AWP:
                guns[2].price = std::to_string(price);
                guns[2].id = "3";
                break;
            default:
                break;
        }
    }

    auto ammo_prices = response.get_ammo_prices();
    std::cerr << "Ammo prices received: " << ammo_prices.size() << std::endl;
    std::cout << "Ammo prices: " << std::endl;
    for (const auto& [ammo_type, price]: ammo_prices) {
        std::cout << "Gun Type: " << static_cast<int>(ammo_type) << ", Price: " << price
                  << std::endl;
        switch (ammo_type) {
            case GunType::AK47:
                ammo[0].price = std::to_string(price);
                ammo[0].id = "4";
                break;
            case GunType::M3:
                ammo[1].price = std::to_string(price);
                ammo[1].id = "5";
                break;
            case GunType::AWP:
                ammo[2].price = std::to_string(price);
                ammo[2].id = "6";
                break;
            case GunType::Glock:
                ammo[3].price = std::to_string(price);
                ammo[3].id = "7";
                break;
            default:
                break;
        }
    }
}

ItemSlot shopDisplay::get_ammo_type(int id_slot) {
    if (id_slot != 7) {
        return ItemSlot::Secondary;
    }
    return ItemSlot::Primary;
}

bool shopDisplay::isActive() { return active; }

void shopDisplay::render() {
    if (active) {
        renderSlots();
        renderItem();
        return;
    }
}

void shopDisplay::renderSlots() {
    int base_x = DISPLAY_WIDTH / 2 - size_slots_w * 2;
    int base_y = DISPLAY_HEIGHT / 2 - size_slots_h * 2;
    for (size_t i = 0; i < (guns.size() + ammo.size()) / 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            Area src(0, 0, size_guns_w, size_guns_h);
            Area dest(base_x + j * (size_guns_w * 2), base_y + i * (size_guns_h * 2),
                      size_guns_w * 2, size_guns_h * 2);
            if (gun_buy == static_cast<int>(i * 2 + j)) {
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

        int x = DISPLAY_WIDTH / 2 - size_slots_w * 2 + 10 + col * (size_guns_w * 2 + 10);
        int y = DISPLAY_HEIGHT / 2 - size_slots_h * 2 + 20 + row * (size_guns_h * 2);

        // Get correct sprite from 4x2 grid
        int sprite_col = i % 4;
        int sprite_row = i / 4;
        Area src(sprite_col * 66, sprite_row * 17, 66, 17);

        Area iconDest(x + 30, y, size_guns_w - 30, size_guns_h + 10);
        gun_icons.render(src, iconDest);

        gunNumber.setTextString(guns[i].id);
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
    for (size_t i = 0; i < ammo.size(); ++i) {
        int row = (guns.size() + i) / 2;
        int col = (guns.size() + i) % 2;

        int x = DISPLAY_WIDTH / 2 - size_slots_w * 2 + 10 + col * (size_guns_w * 2 + 10);
        int y = DISPLAY_HEIGHT / 2 - size_slots_h * 2 + 20 + row * (size_guns_h * 2);

        Area src(0, 0, 60, 30);
        Area iconDest(x + 30, y, size_guns_w - 60, size_guns_h + 10);
        ammo_icons.render(src, iconDest);

        gunNumber.setTextString(ammo[i].id);
        Area numDest(x, y - 10, 15, 15);
        gunNumber.render(numDest);

        if (ammo[i].price.empty()) {
            cost_money.setTextString("Not Available");
        } else {
            cost_money.setTextString("$" + ammo[i].price);
        }
        Area priceDest(x + size_guns_w + 10, y, 100, 40);
        cost_money.render(priceDest);
    }
}

std::optional<Message> shopDisplay::getPurchaseCommand(int x, int y) {
    if (!active) {
        return std::nullopt;
    }
    int base_x = DISPLAY_WIDTH / 2 - size_slots_w * 2;
    int base_y = DISPLAY_HEIGHT / 2 - size_slots_h * 2;

    int slot_width = size_guns_w * 2;
    int slot_height = size_guns_h * 2;

    if (x < base_x || x >= base_x + 2 * slot_width || y < base_y ||
        y >= base_y + (static_cast<int>(guns.size() + ammo.size()) / 2) * slot_height) {
        return std::nullopt;
    }

    int col = (x - base_x) / slot_width;
    int row = (y - base_y) / slot_height;

    int slot_index = row * 2 + col;

    if (GameConfig::GunIndexMap.count(slot_index)) {
        gun_buy = slot_index;
        return Message(BuyGunCommand(GameConfig::GunIndexMap.at(slot_index)));
    } else if (GameConfig::AmmoIndexMap.count(slot_index)) {
        gun_buy = slot_index;
        ItemSlot get_type = get_ammo_type(slot_index);
        return Message(BuyAmmoCommand(get_type));
    }

    return std::nullopt;
}

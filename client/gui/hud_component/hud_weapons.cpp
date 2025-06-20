#include "hud_weapons.h"

#include <cctype>
#include <map>

#include "../../game_config.h"

#include "hud_display.h"

HudWeapons::HudWeapons(SdlWindow& window, const HudLayout& layout):
        window(window),
        equipped_bullets(std::string(GameConfig::Paths::BULLET_PATH), window),
        white_line(std::string(GameConfig::Paths::WHITE_LINE_PATH), window),
        equipped_bullets_amount(window.getRenderer(), std::string(GameConfig::Paths::HUD_NUMS_XCF)),
        gun_number(std::string(GameConfig::Paths::FONT_PATH), 20, {150, 150, 150, 255}, window),
        guns_inventory_texture(std::string(GameConfig::Paths::GUNS_INVENTORY_PATH), window),
        layout(layout) {}

void HudWeapons::render(const GameUpdate& state, const std::string& player_name) {
    render_bullets(state, player_name);
    render_gun_icons(state, player_name);
}

void HudWeapons::render_bullets(const GameUpdate& state, const std::string& player_name) {
    int icon_height = static_cast<int>(32 * layout.scale * 2);
    const Area size_bullets(0, 0, 300, 302);
    const Area dest_bullets(window.getWidth() - window.getWidth() / 40 - layout.padding * 2,
                            window.getHeight() - icon_height * 3, 64 * layout.scale,
                            64 * layout.scale);
    equipped_bullets.render(size_bullets, dest_bullets);

    auto player = state.get_players().at(player_name);
    auto equipped_item = player.get_equipped_item();
    auto player_guns = player.get_inventory().get_guns();

    // If the equipped item is not a gun, return early
    if (player_guns.find(equipped_item) == player_guns.end())
        return;

    auto gun = player_guns.at(equipped_item);
    std::string bullets_str = std::to_string(gun.get_mag_ammo());
    std::string bullets_reserve = std::to_string(gun.get_reserve_ammo());

    int x = window.getWidth() - layout.size_width - window.getWidth() / 40 - layout.padding * 8;
    int y = window.getHeight() - icon_height * 3;
    render_digits(bullets_str, x, y);

    x += layout.digitSpacing * bullets_str.size();

    const Area size_white_line(0, 0, 30, 300);
    const Area dest_white_line(x + 5, y, 5 * layout.scale * 2, 32 * layout.scale * 2);
    white_line.render(size_white_line, dest_white_line);

    x += 5 * layout.scale * 2 + 5;

    render_digits(bullets_reserve, x, y);
}

void HudWeapons::render_gun_icons(const GameUpdate& state, const std::string& player_name) {
    const auto& inventory = state.get_players().at(player_name).get_inventory();
    const auto& guns = inventory.get_guns();

    int x = window.getWidth() - layout.size_width - window.getWidth() / 20;
    int y = window.getHeight() / 2;
    static constexpr int icon_width = 40;
    static constexpr int icon_height = 12;
    static constexpr int spacing = 64;

    std::map<ItemSlot, std::string> slot_to_key = {{ItemSlot::Primary, "1"},
                                                   {ItemSlot::Secondary, "2"},
                                                   {ItemSlot::Melee, "3"},
                                                   {ItemSlot::Bomb, "4"}};

    for (const auto& [slot, gun]: guns) {  // cppcheck-suppress[unassignedVariable]
        GunType type = gun.get_gun();

        int icon_index = -1;
        if (slot == ItemSlot::Primary || slot == ItemSlot::Secondary) {
            std::string key = std::string(GameConfig::GunTypeToStr(type));
            if (GameConfig::offsetInventory.count(key)) {
                icon_index = GameConfig::offsetInventory[key];
            }
        }

        if (icon_index != -1) {
            Area src(icon_index * icon_width, 0, icon_width, icon_height);
            Area dest(x, y, window.getWidth() / 14, 34);

            guns_inventory_texture.render(src, dest);
            gun_number.setTextString(slot_to_key[slot]);
            gun_number.render(Area(window.getWidth() - layout.padding * 2, y, 10, 20));

            y += spacing;
        }
    }

    // Render knife (always present)
    int knife_index = GameConfig::offsetInventory["knife"];
    Area knife_src(knife_index * icon_width, 0, icon_width, icon_height);
    Area knife_dest(x, y, window.getWidth() / 14, 34);
    guns_inventory_texture.render(knife_src, knife_dest);
    gun_number.setTextString(slot_to_key[ItemSlot::Melee]);
    gun_number.render(Area(window.getWidth() - layout.padding * 2, y, 10, 20));

    y += spacing;

    if (!inventory.get_bomb().has_value())
        return;

    int bomb_index = GameConfig::offsetInventory["bomb"];
    Area bomb_src(bomb_index * icon_width, 0, icon_width, icon_height);
    Area bomb_dest(x, y, window.getWidth() / 14, 34);

    guns_inventory_texture.render(bomb_src, bomb_dest);
    gun_number.setTextString(slot_to_key[ItemSlot::Bomb]);
    gun_number.render(Area(window.getWidth() - layout.padding * 2, y, 10, 20));
}

void HudWeapons::render_digits(const std::string& str, int x, int y) {
    for (char c: str) {
        if (isdigit(c)) {
            equipped_bullets_amount.renderDigit(c - '0', x, y, layout.scale);
            x += layout.digitSpacing;
        }
    }
}

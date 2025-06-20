#include "hud_health.h"

#include <cctype>

#include "../../game_config.h"

#include "hud_display.h"

HudHealth::HudHealth(SdlWindow& window, const HudLayout& layout):
        window(window),
        life(std::string(GameConfig::Paths::LIFE_PATH), window),
        life_amount(window.getRenderer(), std::string(GameConfig::Paths::HUD_NUMS_XCF)),
        layout(layout) {}

void HudHealth::render(const GameUpdate& state, const std::string& player_name) {
    int icon_width = static_cast<int>(32 * layout.scale * 2);
    int icon_height = static_cast<int>(32 * layout.scale * 2);

    const Area size_life(0, 0, 62, 64);
    const Area dest_life(layout.padding, window.getHeight() - icon_height - layout.padding,
                         icon_width, icon_height);
    life.render(size_life, dest_life);

    std::string life_str = std::to_string(state.get_players().at(player_name).get_health());
    int x = layout.padding + icon_width + layout.digitSpacing / 2;
    int y = window.getHeight() - icon_height - layout.padding;
    render_digits(life_str, x, y);
}

void HudHealth::render_digits(const std::string& str, int x, int y) {
    for (char c: str) {
        if (isdigit(c)) {
            life_amount.renderDigit(c - '0', x, y, layout.scale);
            x += layout.digitSpacing;
        }
    }
}

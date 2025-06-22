#include "hud_money.h"

#include <cctype>

#include "../../game_config.h"

#include "hud_display.h"

HudMoney::HudMoney(SdlWindow& window, const HudLayout& layout):
        window(window),
        money(std::string(GameConfig::Paths::MONEY_PATH), window),
        money_amount(window.getRenderer(), std::string(GameConfig::Paths::HUD_NUMS_XCF)),
        layout(layout) {}

void HudMoney::render(const GameUpdate& state, const std::string& player_name) {
    int icon_height = static_cast<int>(32 * layout.scale * 2);
    const Area size_money(460, 0, 64, 62);
    const Area dest_money(window.getWidth() - layout.size_width * 2 - layout.padding,
                          window.getHeight() - icon_height - layout.padding, window.getWidth() / 20,
                          icon_height);
    money.render(size_money, dest_money);

    std::string money_str =
            std::to_string(state.get_players().at(player_name).get_inventory().get_money());
    int x = window.getWidth() - layout.size_width - layout.padding * 4;
    int y = window.getHeight() - icon_height - layout.padding;
    render_digits(money_str, x, y);
}

void HudMoney::render_digits(const std::string& str, int x, int y) {
    for (char c: str) {
        if (isdigit(c)) {
            money_amount.renderDigit(c - '0', x, y, layout.scale);
            x += layout.digitSpacing;
        }
    }
}

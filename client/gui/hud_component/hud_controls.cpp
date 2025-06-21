#include "hud_controls.h"

#include <string>

#include "../../game_config.h"

#include "hud_display.h"

HudControls::HudControls(SdlWindow& window, const HudLayout& layout, float scale_ratio):
        window(window),
        start_game_button(std::string(GameConfig::Paths::BACKGROUND_WINNER_PATH), window),
        round_text(std::string(GameConfig::Paths::FONT_PATH), 20, {150, 150, 150, 255}, window),
        layout(layout),
        scale_ratio(scale_ratio) {}

void HudControls::render_start_game_button() {
    int button_width = static_cast<int>(280 * scale_ratio);
    int button_height = static_cast<int>(50 * scale_ratio);
    const Area size_button(0, 0, 680, 130);
    const Area dest_button(window.getWidth() / 2 - button_width / 2,
                           window.getHeight() - button_height - layout.padding * 2, button_width,
                           button_height);
    start_game_button.render(size_button, dest_button);

    round_text.setTextString("START GAME");
    round_text.render(Area(window.getWidth() / 2 - button_width * 0.5 / 2 + layout.padding,
                           window.getHeight() - button_height - layout.padding * 2 + layout.padding,
                           button_width * 0.5 - layout.padding * 2,
                           button_height - layout.padding * 2));
}

bool HudControls::start_game_click(int x, int y) {
    int button_width = static_cast<int>(200 * scale_ratio);
    int button_height = static_cast<int>(50 * scale_ratio);
    int button_x = window.getWidth() / 2 - button_width / 2;
    int button_y = window.getHeight() - button_height - layout.padding * 2;

    if (x >= button_x && x <= button_x + button_width && y >= button_y &&
        y <= button_y + button_height) {
        return true;
    }
    return false;
}

#include "hud_timer.h"

#include <string>

#include "../../game_config.h"

#include "hud_display.h"

HudTimer::HudTimer(SdlWindow& window, const HudLayout& layout):
        window(window),
        timer_amount(window.getRenderer(), std::string(GameConfig::Paths::HUD_NUMS_XCF)),
        timer_dots(std::string(GameConfig::Paths::HUD_NUMS_XCF), window),
        round_text(std::string(GameConfig::Paths::FONT_PATH), 20, {150, 150, 150, 255}, window),
        layout(layout) {}

void HudTimer::render(const GameUpdate& state) {
    int secs_remaining = state.get_phase().get_secs_remaining();
    int minutes_idx = secs_remaining / 60;
    int seconds_idx = secs_remaining % 60;
    int seconds_idx_h = seconds_idx / 10;
    int seconds_idx_l = seconds_idx % 10;

    int total_timer_width = layout.digitSpacing * 4 + 10;
    int x = window.getWidth() / 2 - total_timer_width / 2;
    int y = layout.padding + 30;

    timer_amount.renderDigit(minutes_idx, x, y, layout.scale);
    x += layout.digitSpacing;

    Area src_colon(475, 0, 10, layout.size_height);
    Area dst_colon(x, y, 10, layout.size_height / 2);
    timer_dots.render(src_colon, dst_colon);
    x += layout.digitSpacing;

    timer_amount.renderDigit(seconds_idx_h, x, y, layout.scale);
    x += layout.digitSpacing;
    timer_amount.renderDigit(seconds_idx_l, x, y, layout.scale);
}

void HudTimer::render_round_text(const GameUpdate& state) {
    round_text.setTextString("Round " + std::to_string(state.get_num_rounds()));
    round_text.render(Area(window.getWidth() / 2 - 50, layout.padding, 100, 20));
}

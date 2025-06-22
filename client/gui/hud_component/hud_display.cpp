#include "hud_display.h"

#include <algorithm>
#include <memory>

#include "../../game_config.h"

SdlHud::SdlHud(SdlWindow& window, const GameUpdate& state, const std::string& player_name):
        state(state), player_name(player_name), window(window) {
    initialize_layout();

    // Initialize components
    pointer_component = std::make_unique<HudPointer>(window, layout);
    money_component = std::make_unique<HudMoney>(window, layout);
    health_component = std::make_unique<HudHealth>(window, layout);
    timer_component = std::make_unique<HudTimer>(window, layout);
    weapons_component = std::make_unique<HudWeapons>(window, layout);
    scores_component = std::make_unique<HudScores>(window, layout);
    chat_component = std::make_unique<HudChat>(window, layout);
    controls_component = std::make_unique<HudControls>(window, layout, scale_ratio);
}

void SdlHud::initialize_layout() {
    float BASE_WIDTH = 800.0f;
    float BASE_HEIGHT = 600.0f;

    float width_ratio = window.getWidth() / BASE_WIDTH;
    float height_ratio = window.getHeight() / BASE_HEIGHT;
    scale_ratio = std::min(width_ratio, height_ratio);

    layout.padding = static_cast<int>(10 * scale_ratio);
    layout.iconWidth = static_cast<int>(32 * scale_ratio);
    layout.size_width = static_cast<int>(62 * scale_ratio);
    layout.size_height = static_cast<int>(64 * scale_ratio);
    layout.iconHeight = static_cast<int>(32 * scale_ratio);
    layout.digitSpacing = static_cast<int>(22 * scale_ratio);
    layout.digitHeight = static_cast<int>(32 * scale_ratio);
    layout.scale = 0.5f * scale_ratio;
}

void SdlHud::render() {
    scores_component->render(state);
    pointer_component->render();
    money_component->render(state, player_name);
    health_component->render(state, player_name);
    timer_component->render(state);
    timer_component->render_round_text(state);
    weapons_component->render(state, player_name);
    scores_component->render_mute_icon();

    if (state.get_phase().get_type() == PhaseType::WarmUp) {
        controls_component->render_start_game_button();
        chat_component->render(state);
    }
}

void SdlHud::update_pointer_position(int x, int y) { pointer_component->update_position(x, y); }

bool SdlHud::start_game_click(int x, int y) { return controls_component->start_game_click(x, y); }


void SdlHud::update_mute_icon() { scores_component->update_mute_icon(); }

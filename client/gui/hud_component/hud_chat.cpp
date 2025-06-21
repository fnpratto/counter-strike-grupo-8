#include "hud_chat.h"

#include "../../game_config.h"

#include "hud_display.h"

HudChat::HudChat(SdlWindow& window, const HudLayout& layout):
        window(window),
        round_text(std::string(GameConfig::Paths::FONT_PATH), 20, {150, 150, 150, 255}, window),
        layout(layout) {}

void HudChat::render(const GameUpdate& state) {
    int total_players = state.get_players().size();
    int ready_players = 0;

    for (const auto& [name, player_state]:  // cppcheck-suppress[unassignedVariable]
         state.get_players()) {
        if (player_state.get_ready()) {
            ready_players++;
            round_text.setTextString(name + " join game");

            Area chat_area(layout.padding,
                           window.getHeight() - layout.size_height * 2 -
                                   ready_players * layout.size_height / 2,
                           layout.size_width * 1.2, layout.size_height / 2);
            round_text.render(chat_area);
        }
    }

    std::string ready_status = std::to_string(ready_players) + "/" + std::to_string(total_players);
    round_text.setTextString("Ready: " + ready_status);

    Area status_area(50, layout.size_height, layout.size_width * 2, layout.size_height * 1.2);
    round_text.render(status_area);
}

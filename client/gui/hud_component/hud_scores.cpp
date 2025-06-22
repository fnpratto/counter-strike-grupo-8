#include "hud_scores.h"

#include <string>

#include <SDL2/SDL.h>

#include "../../game_config.h"

#include "hud_display.h"

HudScores::HudScores(SdlWindow& window, const HudLayout& layout):
        window(window),
        score_text(std::string(GameConfig::Paths::FONT_PATH), 20, {255, 255, 255, 255}, window),
        mute_icon(std::string(GameConfig::Paths::MUTE_ICON_PATH), window),
        trapecio(std::string(GameConfig::Paths::TRAPECIO_PATH), window),
        parallelogram_blue(std::string(GameConfig::Paths::PARALELO_BLUE_O_PATH), window),
        parallelogram_red(std::string(GameConfig::Paths::PARALELO_RED_PATH), window),
        layout(layout) {}

void HudScores::render([[maybe_unused]] const GameUpdate& state) {
    render_background();
    render_team_scores();
}

void HudScores::render_background() {
    SDL_Renderer* renderer = window.getRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_Rect rect = {0, 0, window.getWidth(), 10};
    SDL_RenderFillRect(renderer, &rect);

    Area src_trapecio(0, 0, 500, 230);
    Area dest_trapecio(window.getWidth() * 0.5 - window.getWidth() * 0.5 / 2, 0,
                       window.getWidth() * 0.5, layout.size_height * 1.75);
    trapecio.render(src_trapecio, dest_trapecio);
}

void HudScores::render_team_scores() {
    const int trapecio_width = window.getWidth() * 0.5;
    const int trapecio_x = (window.getWidth() - trapecio_width) / 4;

    const int paral_width = layout.size_width * 4;
    const int paral_height = layout.size_height * 1.75;
    const int margin = layout.padding * 2;

    // BLUE (izquierda del trapecio)
    Area src_parallelogram1(0, 180, 500, 320);
    Area dest_parallelogram1(trapecio_x - paral_width / 4 + margin, layout.padding * 2, paral_width,
                             paral_height);
    parallelogram_blue.render(src_parallelogram1, dest_parallelogram1);

    score_text.setTextString(std::to_string(0));  // TODO : Replace with actual score
    score_text.render(Area(trapecio_x - paral_width / 4 + margin * 6, layout.padding * 3,
                           layout.size_width / 1.75, layout.size_height / 2));

    // RED (derecha del trapecio)
    Area src_parallelogram(0, 180, 500, 320);
    Area dest_parallelogram(trapecio_x + trapecio_width + margin, layout.padding * 2, paral_width,
                            paral_height);
    parallelogram_red.render(src_parallelogram, dest_parallelogram);

    score_text.setTextString(std::to_string(0));  // TODO : Replace with actual score
    score_text.render(
            Area(trapecio_x + trapecio_width + margin + paral_width / 2 - layout.size_width / 4,
                 layout.padding * 3, layout.size_width / 1.75, layout.size_height / 2));
}

void HudScores::render_mute_icon() {
    int icon_width = static_cast<int>(32 * layout.scale * 2);
    int icon_height = static_cast<int>(32 * layout.scale * 2);

    if (is_muted) {
        const Area size_mute_icon(0, 0, 64, 64);
        const Area dest_mute_icon(window.getWidth() - icon_width - layout.padding * 4, icon_height,
                                  icon_width, icon_height);
        mute_icon.render(size_mute_icon, dest_mute_icon);
    } else {
        const Area size_mute_icon(256, 0, 64, 64);
        const Area dest_mute_icon(window.getWidth() - icon_width - layout.padding * 4, icon_height,
                                  icon_width, icon_height);
        mute_icon.render(size_mute_icon, dest_mute_icon);
    }
}

void HudScores::update_mute_icon() { is_muted = !is_muted; }

#ifndef HUD_SCORES_H
#define HUD_SCORES_H

#include "../window_elements/sdl_text.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/updates/game_update.h"

struct HudLayout;

class HudScores {
private:
    SdlWindow& window;
    SdlText score_text;
    SdlTexture mute_icon;
    SdlTexture trapecio;
    SdlTexture parallelogram_blue;
    SdlTexture parallelogram_red;
    const HudLayout& layout;
    bool is_muted = false;
    int score_ct = 0;
    int score_tt = 0;

public:
    explicit HudScores(SdlWindow& window, const HudLayout& layout);
    void render(const GameUpdate& state);
    void render_mute_icon();
    void update_mute_icon();
    void update__winner_score(Team winner);

private:
    void render_background();
    void render_team_scores();
};

#endif  // HUD_SCORES_H

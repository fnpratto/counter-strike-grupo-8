#ifndef LIST_TEAMS_H
#define LIST_TEAMS_H

#include <optional>
#include <string>
#include <vector>

#include "../hud_component/bit_map_font_number.h"
#include "../window_elements/area.h"
#include "../window_elements/sdl_text.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/commands.h"
#include "common/message.h"
#include "common/models.h"

class listTeams {
public:
    explicit listTeams(SdlWindow& window);
    void update(int currentClockTick);
    std::optional<Team> updatePointerPosition(int x, int y);

private:
    SdlWindow& window;
    int DISPLAY_WIDTH;
    int DISPLAY_HEIGHT;
    int size_slots_w;
    int size_slots_h;
    int base_x;
    int base_y;
    SdlText text;
    SdlText smaller_text;
    SdlTexture rectangulo_horizontal;
    SdlTexture background;
    SdlTexture terrorist;
    SdlTexture counter_terrorist;
    BitmapFont timer_amount;
    SdlTexture timer_dots;
    bool selected_team;
    int heightRatio;
    int size_height;
    int widthRatio;
    int scaleRatio;
    int digitSpacing;
    float scale;
    void render_timer(int currentClockTick);
    void renderSlots();
    void renderItem();
};

#endif  // LIST_TEAMS

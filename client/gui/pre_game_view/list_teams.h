#ifndef LIST_TEAMS_H
#define LIST_TEAMS_H

#include <string>
#include <vector>

#include "../hud_component/BitmapFontNumber.h"
#include "../window_elements/Area.h"
#include "../window_elements/SdlText.h"
#include "../window_elements/SdlTexture.h"
#include "../window_elements/SdlWindow.h"


class listTeams {
public:
    explicit listTeams(SdlWindow& window);
    void update(int currentClockTick);
    void updatePointerPosition(int x, int y);

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

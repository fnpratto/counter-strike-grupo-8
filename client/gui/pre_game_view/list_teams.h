#ifndef LIST_TEAMS_H
#define LIST_TEAMS_H

#include <atomic>
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
    void render();
    std::optional<Team> updatePointerPosition(int x, int y);
    bool isActive();

private:
    SdlWindow& window;
    int DISPLAY_WIDTH;
    int DISPLAY_HEIGHT;
    int size_slots_w;
    int size_slots_h;
    int size_image;
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
    std::atomic<bool> active;
    float terrorist_x, terrorist_y;
    float counter_terrorist_x, counter_terrorist_y;
    float slot_width, slot_height;
    int select_skin_x;
    int select_skin_y;
    int select_skin_width;
    int select_skin_height;

    void render_button();
    void renderSlots();
    void renderItem();
};

#endif  // LIST_TEAMS

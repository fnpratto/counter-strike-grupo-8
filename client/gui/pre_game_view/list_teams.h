#ifndef LIST_TEAMS_H
#define LIST_TEAMS_H

#include <string>
#include <vector>

#include "../window_elements/Area.h"
#include "../window_elements/SdlText.h"
#include "../window_elements/SdlTexture.h"
#include "../window_elements/SdlWindow.h"


class listTeams {
public:
    explicit listTeams(SdlWindow& window);
    void render();
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
    bool selected_team;
    void renderSlots();
    void renderItem();
    void renderName(int index, int team);
};

#endif  // LIST_TEAMS

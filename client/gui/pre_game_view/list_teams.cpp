#include "list_teams.h"

#include <algorithm>
#include <iostream>

const std::string& RECTANGULE_HORIZONTAL = "../assets/gfx/listTeams/rectanguloxcf.xcf";
const std::string& SHADE_PATH = "../assets/gfx/listTeams/gui_shade.bmp";
const std::string& BACKGROUND_PATH_1 = "../assets/gfx/listTeams/rusty-black.xcf";
const int padding = 10;
const std::string& TEXT_PATH = "../assets/gfx/fonts/Bebas_Neue/BebasNeue-Regular.ttf";
const std::string& TERRORIST_PATH = "../assets/gfx/listTeams/terrorist.png";
const std::string& COUNTER_TERRORIST_PATH = "../assets/gfx/listTeams/counter-terrorist.png";
const std::string& SMALLER_TEXT_PATH = "../assets/gfx/fonts/HeadingNowTrial-03Book.ttf";


listTeams::listTeams(SdlWindow& window, const GameUpdate& state, const std::string& player_name):
        window(window),
        game_state(state),
        player_name(player_name),
        DISPLAY_WIDTH(window.getWidth()),
        DISPLAY_HEIGHT(window.getHeight()),
        text(TEXT_PATH, 100, {255, 255, 255, 255}, window),
        smaller_text(SMALLER_TEXT_PATH, 100, {255, 255, 255, 255}, window),
        team_labels{{SdlText(TEXT_PATH, 100, {255, 255, 255, 255}, window),
                     SdlText(TEXT_PATH, 100, {255, 255, 255, 255}, window)}},
        tt_descriptions{SdlText(SMALLER_TEXT_PATH, 100, {255, 255, 255, 255}, window),
                        SdlText(SMALLER_TEXT_PATH, 100, {255, 255, 255, 255}, window),
                        SdlText(SMALLER_TEXT_PATH, 100, {255, 255, 255, 255}, window)},
        ct_descriptions{SdlText(SMALLER_TEXT_PATH, 100, {255, 255, 255, 255}, window),
                        SdlText(SMALLER_TEXT_PATH, 100, {255, 255, 255, 255}, window),
                        SdlText(SMALLER_TEXT_PATH, 100, {255, 255, 255, 255}, window)},
        rectangulo_horizontal(RECTANGULE_HORIZONTAL, window),
        background(BACKGROUND_PATH_1, window),
        terrorist(TERRORIST_PATH, window),
        counter_terrorist(COUNTER_TERRORIST_PATH, window),
        timer_amount(window.getRenderer(), "../assets/gfx/fonts/hud_nums.xcf"),
        timer_dots("../assets/gfx/fonts/hud_nums.xcf", window) {
    float BASE_WIDTH = 800.0f;
    float BASE_HEIGHT = 600.0f;
    widthRatio = DISPLAY_WIDTH / BASE_WIDTH;
    heightRatio = DISPLAY_HEIGHT / BASE_HEIGHT;
    size_slots_h = 400 * heightRatio;
    size_slots_w = 200 * widthRatio;
    base_x = DISPLAY_WIDTH / 2 - size_slots_w;
    base_y = DISPLAY_HEIGHT * 0.2;

    scaleRatio = std::min(widthRatio, heightRatio);
    digitSpacing = static_cast<int>(22 * scaleRatio);
    size_height = static_cast<int>(64 * scaleRatio);
    scale = 0.5f * scaleRatio;
    size_image = size_slots_w;
    active = true;

    terrorist_x = DISPLAY_WIDTH / 2 - DISPLAY_WIDTH * 0.30 - padding;
    terrorist_y = base_y;
    counter_terrorist_x = DISPLAY_WIDTH / 2 + padding;
    counter_terrorist_y = base_y;
    slot_width = DISPLAY_WIDTH * 0.30;
    slot_height = DISPLAY_WIDTH * 0.30;
    //
    select_skin_width = 200 * widthRatio;
    select_skin_height = 50 * heightRatio;
    select_skin_x = DISPLAY_WIDTH - select_skin_width - padding * 4;
    select_skin_y = padding * 4;
    // Set preloaded labels
    team_labels[0].setTextString("Terrorist");
    team_labels[1].setTextString("Counter-Terrorist");

    // Set TT descriptions
    tt_descriptions[0].setTextString("Plant the bomb and defend it until ");
    tt_descriptions[1].setTextString("it explodes, or eliminate all ");
    tt_descriptions[2].setTextString("Counter-Terrorists to secure victory.");

    // Set CT descriptions
    ct_descriptions[0].setTextString("Prevent the terrorist from ");
    ct_descriptions[1].setTextString("detonating their bomb or ");
    ct_descriptions[2].setTextString("eliminate them all to win.");
}


void listTeams::render() {
    if (active) {
        Area src(0, 0, 250, 250);
        Area dest(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
        background.render(src, dest);

        text.setTextString("CHOOSE TEAM");

        Area priceDest(0 + padding * 8, 0 + padding * 4, 150, 50);
        text.render(priceDest);
        Area src1(0, 0, 600, 300);
        Area dest1(padding, base_y - 50, DISPLAY_WIDTH, DISPLAY_WIDTH * 0.40 + 50);

        rectangulo_horizontal.render(src1, dest1);
        renderSlots();
        render_button();
    }
}

bool listTeams::isActive() { return active; }

void listTeams::render_button() {

    SDL_Rect button_rect = {select_skin_x, select_skin_y, select_skin_width, select_skin_height};

    SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 0, 255);
    SDL_RenderFillRect(window.getRenderer(), &button_rect);

    // Dibujar borde blanco (opcional)
    SDL_SetRenderDrawColor(window.getRenderer(), 255, 255, 255, 255);
    SDL_RenderDrawRect(window.getRenderer(), &button_rect);

    // Dibujar texto encima del botón
    text.setTextString("Select Skin");
    Area text_area(select_skin_x + 10, select_skin_y + 10, 180, 30);
    text.render(text_area);
}

void listTeams::renderSlots() {

    Area src(0, 0, 375, 410);
    Area src1(0, 5, 375, 460);
    Area terrorist_dest(terrorist_x, terrorist_y, slot_width, slot_height);
    Area counter_terrorist_dest(counter_terrorist_x, counter_terrorist_y, slot_width, slot_height);

    terrorist.render(src, terrorist_dest);
    counter_terrorist.render(src1, counter_terrorist_dest);

    Area terrorist_text_dest(terrorist_x + size_slots_w / 4,
                             terrorist_y + DISPLAY_WIDTH * 0.30 - 50, 150, 50);
    team_labels[0].render(terrorist_text_dest);

    Area counter_text_dest(counter_terrorist_x + size_slots_w / 4,
                           counter_terrorist_y + DISPLAY_WIDTH * 0.30 - 50, 200, 50);
    team_labels[1].render(counter_text_dest);

    std::optional<Team> team_choosen = game_state.get_players().at(player_name).get_optional_team();

    int base_y_text = team_choosen == Team::TT ? terrorist_y + DISPLAY_WIDTH * 0.30 :
                                                 counter_terrorist_y + DISPLAY_WIDTH * 0.30;

    int base_x_text = team_choosen == Team::TT ? terrorist_x + size_slots_w / 4 :
                                                 counter_terrorist_x + size_slots_w / 4;

    for (int i = 0; i < 3; ++i) {
        Area line_area(base_x_text + padding, base_y_text + padding * (i * 3), 400 * scale, 50);
        if (team_choosen == Team::TT) {
            tt_descriptions[i].render(line_area);
        } else {
            ct_descriptions[i].render(line_area);
        }
    }
}


std::optional<Team> listTeams::updatePointerPosition(int x, int y) {

    if (x >= select_skin_x && x <= select_skin_x + select_skin_width && y >= select_skin_y &&
        y <= select_skin_y + select_skin_height) {
        active = false;
    }

    // Check if the pointer is over the terrorist slot
    if (x >= terrorist_x && x <= terrorist_x + slot_width && y >= terrorist_y &&
        y <= terrorist_y + slot_height) {
        return Team::TT;
    }

    // Check if the pointer is over the counter-terrorist slot
    if (x >= counter_terrorist_x && x <= counter_terrorist_x + slot_width &&
        y >= counter_terrorist_y && y <= counter_terrorist_y + slot_height) {
        return Team::CT;
    }
    return std::nullopt;
}

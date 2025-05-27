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


listTeams::listTeams(SdlWindow& window):
        window(window),
        DISPLAY_WIDTH(window.getWidth()),
        DISPLAY_HEIGHT(window.getHeight()),
        rectangulo_horizontal(RECTANGULE_HORIZONTAL, window),
        background(BACKGROUND_PATH_1, window),
        text(TEXT_PATH, 100, {255, 255, 255, 255}, window),
        smaller_text(SMALLER_TEXT_PATH, 100, {255, 255, 255, 255}, window),
        terrorist(TERRORIST_PATH, window),
        counter_terrorist(COUNTER_TERRORIST_PATH, window),
        timer_amount(window.getRenderer(), "../assets/gfx/fonts/hud_nums.xcf"),
        timer_dots("../assets/gfx/fonts/hud_nums.xcf", window) {
    selected_team = 0;

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
}

void listTeams::update(int currentClockTick) {
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
    int time_left = 5 - currentClockTick;
    if (time_left < 0) {
        time_left = 0;
    }
    render_timer(time_left);
}

void listTeams::render_timer(int currentClockTick) {
    int minutesIdx = std::floor(currentClockTick / 60);
    int seconds = currentClockTick % 60;
    int secondsIdxH = std::floor(seconds / 10);
    int secondsIdxL = seconds % 10;

    int totalTimerWidth = size_slots_w;
    int x = DISPLAY_WIDTH - totalTimerWidth;
    int y = padding * 4.5;

    timer_amount.renderDigit(minutesIdx, x, y, scale);
    x += digitSpacing;

    Area srcColon(475, 0, 10, size_height);
    Area dstColon(x, y, 10, size_height / 2);
    timer_dots.render(srcColon, dstColon);
    x += digitSpacing;

    timer_amount.renderDigit(secondsIdxH, x, y, scale);
    x += digitSpacing;
    timer_amount.renderDigit(secondsIdxL, x, y, scale);

    smaller_text.setTextString("Time left to choose ");
    Area timeDest(DISPLAY_WIDTH - totalTimerWidth - padding - 250, padding * 4, 250, 40);
    smaller_text.render(timeDest);
}

void listTeams::renderSlots() {
    // Calculate the positions for the terrorist and counter-terrorist images
    int terrorist_x = DISPLAY_WIDTH / 2 - DISPLAY_WIDTH * 0.30 - padding;
    int terrorist_y = base_y;
    int counter_terrorist_x = DISPLAY_WIDTH / 2 + padding;
    int counter_terrorist_y = base_y;

    Area src(0, 0, 375, 410);
    Area src1(0, 5, 375, 460);
    Area terrorist_dest(terrorist_x, terrorist_y, DISPLAY_WIDTH * 0.30, DISPLAY_WIDTH * 0.30);
    Area counter_terrorist_dest(counter_terrorist_x, counter_terrorist_y, DISPLAY_WIDTH * 0.30,
                                DISPLAY_WIDTH * 0.30);

    // Render the terrorist and counter-terrorist images
    terrorist.render(src, terrorist_dest);
    counter_terrorist.render(src1, counter_terrorist_dest);

    // Add text below the terrorist image
    text.setTextString("Terrorist");
    Area terrorist_text_dest(terrorist_x + (size_slots_w) / 4,
                             terrorist_y + DISPLAY_WIDTH * 0.30 - 50, 150, 50);
    text.render(terrorist_text_dest);

    // Add text below the counter-terrorist image
    text.setTextString("Counter-Terrorist");
    Area counter_terrorist_text_dest(counter_terrorist_x + (size_slots_w) / 4,
                                     counter_terrorist_y + DISPLAY_WIDTH * 0.30 - 50, 200, 50);
    text.render(counter_terrorist_text_dest);

    terrorist_y += DISPLAY_WIDTH * 0.30;
    counter_terrorist_y += DISPLAY_WIDTH * 0.30;
    terrorist_x += (size_slots_w) / 4;
    counter_terrorist_x += (size_slots_w) / 4;
    // Render additional descriptive text for both teams
    if (selected_team == 1) {
        smaller_text.setTextString("Plant the bomb and defend it until ");
        Area terrorist_text_dest_1(terrorist_x + padding, terrorist_y, 400 * scale, 50);
        smaller_text.render(terrorist_text_dest_1);

        smaller_text.setTextString("it explodes, or eliminate all ");
        Area terrorist_text_dest_2(terrorist_x + padding, terrorist_y + padding * 3, 400 * scale,
                                   50);
        smaller_text.render(terrorist_text_dest_2);

        smaller_text.setTextString("Counter-Terrorists to secure victory.");
        Area terrorist_text_dest_3(terrorist_x + padding, terrorist_y + padding * 6, 400 * scale,
                                   50);
        smaller_text.render(terrorist_text_dest_3);
    } else {
        smaller_text.setTextString("Prevent the terrorist from ");
        Area counter_terrorist_text_dest_1(counter_terrorist_x + padding, counter_terrorist_y,
                                           400 * scale, 50);
        smaller_text.render(counter_terrorist_text_dest_1);

        smaller_text.setTextString("detonating their bomb or ");
        Area counter_terrorist_text_dest_2(counter_terrorist_x + padding,
                                           counter_terrorist_y + padding * 3, 400 * scale, 50);
        smaller_text.render(counter_terrorist_text_dest_2);

        smaller_text.setTextString("eliminate them all to win.");
        Area counter_terrorist_text_dest_3(counter_terrorist_x + padding,
                                           counter_terrorist_y + padding * 6, 400 * scale, 50);
        smaller_text.render(counter_terrorist_text_dest_3);
    }
}
void listTeams::updatePointerPosition(int x, int y) {
    int terrorist_x = base_x - size_slots_w / 2 - padding;
    int terrorist_y = base_y;
    int counter_terrorist_x = base_x + size_slots_w;
    int counter_terrorist_y = base_y;

    int slot_width = size_slots_w * 1.50;
    int slot_height = size_slots_w * 1.7;

    // Check if the pointer is over the terrorist slot
    if (x >= terrorist_x && x <= terrorist_x + slot_width && y >= terrorist_y &&
        y <= terrorist_y + slot_height) {
        std::cerr << "Mouse is over Terrorist slot." << std::endl;
        selected_team = 1;  // terrorist
        return;
    }

    // Check if the pointer is over the counter-terrorist slot
    if (x >= counter_terrorist_x && x <= counter_terrorist_x + slot_width &&
        y >= counter_terrorist_y && y <= counter_terrorist_y + slot_height) {
        std::cerr << "Mouse is over Counter-Terrorist slot." << std::endl;
        selected_team = 0;  // counter
        return;
    }
}

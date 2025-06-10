#include "list_skins.h"

#include <iostream>

const std::string& TEXT1_PATH = "../assets/gfx/fonts/Bebas_Neue/BebasNeue-Regular.ttf";
const std::string& BACKGROUND1_PATH = "../assets/gfx/listTeams/rusty-black.xcf";


const std::vector<std::string> SKIN_PATHS = {
        "../assets/gfx/player/ct1.xcf", "../assets/gfx/player/ct2.bmp",
        "../assets/gfx/player/ct3.bmp", "../assets/gfx/player/ct4.bmp",
        "../assets/gfx/player/t1.bmp",  "../assets/gfx/player/t2.bmp",
        "../assets/gfx/player/t3.bmp",  "../assets/gfx/player/t4.bmp"};

skinSelect::skinSelect(SdlWindow& window):
        window(window),
        DISPLAY_WIDTH(window.getWidth()),
        DISPLAY_HEIGHT(window.getHeight()),
        text(TEXT1_PATH, 100, {255, 255, 255, 255}, window),
        background(BACKGROUND1_PATH, window),
        selected_skin(-1),
        active(true) {

    float BASE_WIDTH = 800.0f;
    float BASE_HEIGHT = 600.0f;

    widthRatio = DISPLAY_WIDTH / BASE_WIDTH;
    heightRatio = DISPLAY_HEIGHT / BASE_HEIGHT;
    scaleRatio = std::min(widthRatio, heightRatio);

    base_x = DISPLAY_WIDTH * 0.1f;
    base_y = DISPLAY_HEIGHT * 0.2f;

    slot_width = DISPLAY_WIDTH * 0.18f;
    slot_height = slot_width;

    size_image = slot_width;

    select_skin_width = 200 * widthRatio;
    select_skin_height = 50 * heightRatio;
    select_skin_x = DISPLAY_WIDTH - select_skin_width - padding * 4;
    select_skin_y = padding * 4;


    std::transform(SKIN_PATHS.begin(), SKIN_PATHS.end(), std::back_inserter(skins),
                   [&window](const std::string& path) { return SdlTexture(path, window); });
}

void skinSelect::render() {
    if (!active)
        return;

    Area src(0, 0, 250, 250);
    Area dest(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    background.render(src, dest);

    text.setTextString("CHOOSE YOUR SKIN");
    Area title_area(0 + padding * 8, 0 + padding * 4, 400, 50);
    text.render(title_area);

    renderSkins();
    renderButton();
}

void skinSelect::renderSkins() {
    Area src(0, 0, 32, 32);

    // TODO depende el game estate equipo eligo las skins si ct primeras 4 si tt las segundas 4
    for (size_t i = 0; i < 4 && i < skins.size(); ++i) {
        float x = base_x + i * (slot_width + padding);
        float y = base_y;
        Area dest(x, y, slot_width, slot_height);

        skins[i].render(src, dest);
        // Highlight selected skin
        if (selected_skin == static_cast<int>(i)) {
            SDL_Rect highlight = {static_cast<int>(x), static_cast<int>(y),
                                  static_cast<int>(slot_width), static_cast<int>(slot_height)};
            SDL_SetRenderDrawColor(window.getRenderer(), 255, 255, 0, 255);  // Yellow
            SDL_RenderDrawRect(window.getRenderer(), &highlight);
        }

        // Draw skin label
        text.setTextString("Skin " + std::to_string(i + 1));
        Area label_area(x + 10, y + slot_height + 10, 100, 30);
        text.render(label_area);
    }
}

void skinSelect::renderButton() {
    SDL_Rect button_rect = {select_skin_x, select_skin_y, select_skin_width, select_skin_height};
    SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 0, 255);
    SDL_RenderFillRect(window.getRenderer(), &button_rect);
    SDL_SetRenderDrawColor(window.getRenderer(), 255, 255, 255, 255);
    SDL_RenderDrawRect(window.getRenderer(), &button_rect);

    if (selected_skin == -1) {
        text.setTextString("Select Skin");
    } else {
        text.setTextString("Join Warm-Up: " + std::to_string(selected_skin + 1));
    }

    Area text_area(select_skin_x + 10, select_skin_y + 10, 180, 30);
    text.render(text_area);
}

bool skinSelect::isActive() { return active; }

std::optional<int> skinSelect::updatePointerPosition(int x, int y) {
    // Button clicked
    if (x >= select_skin_x && x <= select_skin_x + select_skin_width && y >= select_skin_y &&
        y <= select_skin_y + select_skin_height) {
        std::cout << "Join Warm-Up: " << selected_skin << std::endl;
        active = false;
        return selected_skin;
    }

    // Check skin slots
    for (int i = 0; i < 4; ++i) {
        float slot_x = base_x + i * (slot_width + padding);
        float slot_y = base_y;

        if (x >= slot_x && x <= slot_x + slot_width && y >= slot_y && y <= slot_y + slot_height) {
            selected_skin = i;
            return selected_skin;
        }
    }

    return std::nullopt;
}

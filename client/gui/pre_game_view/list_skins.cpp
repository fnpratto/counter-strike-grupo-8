#include "list_skins.h"

#include <iostream>

const std::string& TEXT1_PATH = "../assets/gfx/fonts/Bebas_Neue/BebasNeue-Regular.ttf";
const std::string& BACKGROUND1_PATH = "../assets/gfx/listTeams/rusty-black.xcf";


const std::string& SKIN_PATHS_TT = {"../assets/gfx/player/tt.xcf"};
const std::string& SKIN_PATHS_CT = {"../assets/gfx/player/ct.xcf"};


skinSelect::skinSelect(SdlWindow& window, const GameUpdate& state, const std::string& player_name):
        window(window),
        game_state(state),
        player_name(player_name),
        DISPLAY_WIDTH(window.getWidth()),
        DISPLAY_HEIGHT(window.getHeight()),
        text(TEXT1_PATH, 100, {255, 255, 255, 255}, window),
        background(BACKGROUND1_PATH, window),
        skins_tt(SKIN_PATHS_TT, window),
        skins_ct(SKIN_PATHS_CT, window),
        skin_labels{{SdlText(TEXT1_PATH, 100, {255, 255, 255, 255}, window),
                     SdlText(TEXT1_PATH, 100, {255, 255, 255, 255}, window),
                     SdlText(TEXT1_PATH, 100, {255, 255, 255, 255}, window),
                     SdlText(TEXT1_PATH, 100, {255, 255, 255, 255}, window)}},
        selected_skin(-1),
        active(true) {


    for (int i = 0; i < 4; ++i) {
        skin_labels[i].setTextString("Skin " + std::to_string(i + 1));
    }


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

    select_skin_width = 100 * widthRatio;
    select_skin_height = 50 * heightRatio;
    select_skin_x = DISPLAY_WIDTH - select_skin_width - padding * 4;
    select_skin_y = padding * 4;
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
    // Area src(0, 0, 32, 32);
    std::optional<Team> team_choosen = game_state.get_players().at(player_name).get_optional_team();

    const SdlTexture& selected_texture = (team_choosen.value() == Team::TT) ? skins_tt : skins_ct;

    // TODO depende el game estate equipo eligo las skins si ct primeras 4 si tt las segundas 4
    const int skins_per_row = 2;  // Number of skins per row in the spritesheet
    const int skin_size = 32;     // Width and height of each skin in the sheet

    for (size_t i = 0; i < 4; ++i) {
        int sprite_row = i / skins_per_row;
        int sprite_col = i % skins_per_row;

        // Source area to extract from the texture
        Area src(sprite_col * skin_size, sprite_row * skin_size, skin_size, skin_size);

        float x = base_x + i * (slot_width + padding);
        float y = base_y;
        Area dest(x, y, slot_width, slot_height);

        selected_texture.render(src, dest);

        // Highlight selected skin
        if (selected_skin == static_cast<int>(i)) {
            SDL_Rect highlight = {static_cast<int>(x), static_cast<int>(y),
                                  static_cast<int>(slot_width), static_cast<int>(slot_height)};
            SDL_SetRenderDrawColor(window.getRenderer(), 255, 255, 0, 255);  // Yellow
            SDL_RenderDrawRect(window.getRenderer(), &highlight);
        }

        Area label_area(x + 10, y + slot_height + 10, 100, 30);
        skin_labels[i].render(label_area);
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
        text.setTextString("Join Warm-Up");
    }

    Area text_area(select_skin_x + 10, select_skin_y + 10, 180, 30);
    text.render(text_area);
}

bool skinSelect::isActive() { return active; }

std::optional<CharacterType> skinSelect::updatePointerPosition(int x, int y) {
    // Button clicked
    std::optional<Team> team_choosen = game_state.get_players().at(player_name).get_optional_team();
    // Check skin slots
    for (int i = 0; i < 4; ++i) {
        float slot_x = base_x + i * (slot_width + padding);
        float slot_y = base_y;

        if (x >= slot_x && x <= slot_x + slot_width && y >= slot_y && y <= slot_y + slot_height) {
            selected_skin = i;
            if (team_choosen == Team::CT) {
                switch (i) {
                    case 0:
                        return CharacterType::Seal_Force;
                    case 1:
                        return CharacterType::German_GSG_9;
                    case 2:
                        return CharacterType::UK_SAS;
                    case 3:
                        return CharacterType::French_GIGN;
                }
            } else if (team_choosen.value() == Team::TT) {
                switch (i) {
                    case 0:
                        return CharacterType::Pheonix;
                    case 1:
                        return CharacterType::L337_Krew;
                    case 2:
                        return CharacterType::Arctic_Avenger;
                    case 3:
                        return CharacterType::Guerrilla;
                }
            }
        }
    }

    return std::nullopt;
}

std::optional<bool> skinSelect::updateFinishPreGame(int x, int y) {
    if (x >= select_skin_x && x <= select_skin_x + select_skin_width && y >= select_skin_y &&
        y <= select_skin_y + select_skin_height) {
        active = false;
        return true;
    }
    return std::nullopt;
}

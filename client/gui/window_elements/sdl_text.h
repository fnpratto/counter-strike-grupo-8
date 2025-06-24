#ifndef SDLTEXT_H
#define SDLTEXT_H

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "area.h"
#include "sdl_window.h"

class SdlText {
private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    TTF_Font* font;
    std::string current_text;
    SDL_Color color;

public:
    SdlText(const std::string& fontFile, int fontSize, SDL_Color color, const SdlWindow& window);
    ~SdlText();
    void setTextString(const std::string& text);
    int render(const Area& dest) const;
    SDL_Texture* getText() const { return texture; }
    SDL_Texture* createTextureFromText(const std::string& text);
};

#endif  // SDLTEXT_H

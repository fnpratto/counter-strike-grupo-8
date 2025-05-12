#ifndef SDLTEXT_H
#define SDLTEXT_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Area.h"
#include "SdlWindow.h"

class SdlText {
private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    TTF_Font* font;
    SDL_Color color;

    SDL_Texture* createTextureFromText(const std::string& text);

public:
    SdlText(const std::string& fontFile, int fontSize, SDL_Color color, const SdlWindow& window);
    ~SdlText();
    void setTextString(const std::string& text);
    int render(const Area& dest) const;
    SDL_Texture* getText() const { return texture; }  // Added getter for texture
};

#endif  // SDLTEXT_H

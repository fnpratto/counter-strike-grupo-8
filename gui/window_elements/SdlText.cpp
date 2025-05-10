#include "SdlText.h"

#include <stdexcept>

#include <SDL2/SDL_ttf.h>

#include "SdlException.h"

SdlText::SdlText(const std::string& fontFile, int fontSize, SDL_Color color,
                 const SdlWindow& window):
        renderer(window.getRenderer()), texture(nullptr), color(color) {
    font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (!font) {
        throw SdlException("Error al cargar la fuente", TTF_GetError());
    }
}

SdlText::~SdlText() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
    if (font) {
        TTF_CloseFont(font);
    }
}

SDL_Texture* SdlText::createTextureFromText(const std::string& text) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) {
        throw SdlException("Error al crear la superficie de texto", TTF_GetError());
    }

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!newTexture) {
        throw SdlException("Error al crear la textura del texto", SDL_GetError());
    }

    return newTexture;
}

void SdlText::setText(const std::string& text) {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
    texture = createTextureFromText(text);
}

int SdlText::render(const Area& dest) const {
    SDL_Rect sdlDest = {dest.getX(), dest.getY(), dest.getWidth(), dest.getHeight()};
    return SDL_RenderCopy(renderer, texture, nullptr, &sdlDest);
}

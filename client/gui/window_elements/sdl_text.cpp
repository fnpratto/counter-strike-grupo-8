#include "sdl_text.h"

#include <stdexcept>

#include <SDL_ttf.h>

#include "sdl_exception.h"

SdlText::SdlText(const std::string& fontFile, int fontSize, SDL_Color color,
                 const SdlWindow& window):
        renderer(window.getRenderer()), texture(nullptr), font(nullptr), color(color) {

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        throw SdlException("Error al inicializar SDL_ttf", TTF_GetError());
    }
    font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
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
    if (!font) {
        throw SdlException("Font not initialized", "Null pointer");
    }
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


void SdlText::setTextString(const std::string& str) {
    if (str == current_text)
        return;  // No change
    current_text = str;

    if (texture)
        SDL_DestroyTexture(texture);

    SDL_Surface* surface = TTF_RenderText_Blended(font, current_text.c_str(), color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

int SdlText::render(const Area& dest) const {
    if (!texture) {
        // Avoid crash if text wasn't set
        return -1;
    }
    SDL_Rect sdlDest = {dest.getX(), dest.getY(), dest.getWidth(), dest.getHeight()};
    return SDL_RenderCopy(renderer, texture, nullptr, &sdlDest);
}

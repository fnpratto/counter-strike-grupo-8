#include "sdl_texture.h"

#include <iostream>
#include <string>

#include <SDL_image.h>

#include "sdl_exception.h"
#include "sdl_window.h"

SdlTexture::SdlTexture(const std::string& filename, const SdlWindow& window):
        renderer(window.getRenderer()) {
    this->texture = loadTexture(filename);
}

// for animations
SdlTexture::SdlTexture(const std::string& filename, const SdlWindow& window, int width, int height):
        renderer(window.getRenderer()), width(width), height(height) {
    this->texture = loadTexture(filename);
}

SdlTexture::~SdlTexture() { SDL_DestroyTexture(this->texture); }

SDL_Texture* SdlTexture::loadTexture(const std::string& filename) {
    SDL_Texture* loadedTexture = IMG_LoadTexture(this->renderer, filename.c_str());
    if (!loadedTexture) {
        std::cout << "Error al cargar la textura: " << filename << std::endl;
        throw SdlException("Error al cargar la textura", SDL_GetError());
    }
    return loadedTexture;
}

int SdlTexture::render(const Area& src, const Area& dest) const {
    SDL_Rect sdlSrc = {src.getX(), src.getY(), src.getWidth(), src.getHeight()};
    SDL_Rect sdlDest = {dest.getX(), dest.getY(), dest.getWidth(), dest.getHeight()};

    return SDL_RenderCopy(this->renderer, this->texture, &sdlSrc, &sdlDest);
}


// for animations
void SdlTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center,
                        SDL_RendererFlip flip) {
    // Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, width, height};

    // Set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    // Render to screen
    if (SDL_RenderCopyEx(this->renderer, this->texture, clip, &renderQuad, angle, center, flip) !=
        0) {
        std::cout << "Render failed: " << SDL_GetError() << std::endl;
    }
}

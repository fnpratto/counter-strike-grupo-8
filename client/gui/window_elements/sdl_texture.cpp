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

SdlTexture::SdlTexture(const std::string& filename, const SdlWindow& window, int width, int height,
                       float scale):
        renderer(window.getRenderer()), width(width), height(height), scale(scale) {
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
    SDL_Rect renderQuad = {x, y, static_cast<int>(width * scale), static_cast<int>(height * scale)};

    if (clip != NULL) {
        renderQuad.w = static_cast<int>(clip->w * scale);
        renderQuad.h = static_cast<int>(clip->h * scale);
    }

    if (SDL_RenderCopyEx(this->renderer, this->texture, clip, &renderQuad, angle, center, flip) !=
        0) {
        std::cout << "Render failed: " << SDL_GetError() << std::endl;
    }
}

void SdlTexture::render(int x, int y, double angle) { render(x, y, nullptr, angle); }


void SdlTexture::render(const Area& src, const Area& dest, double angle) const {
    SDL_Rect sdlSrc = {src.getX(), src.getY(), src.getWidth(), src.getHeight()};
    SDL_Rect sdlDest = {dest.getX(), dest.getY(), dest.getWidth(), dest.getHeight()};

    if (SDL_RenderCopyEx(this->renderer, this->texture, &sdlSrc, &sdlDest, angle, nullptr,
                         SDL_FLIP_NONE) != 0) {
        std::cout << "Render failed: " << SDL_GetError() << std::endl;
    }
}

#include "bit_map_font_number.h"

#include <stdexcept>

#include <SDL2/SDL_image.h>

BitmapFont::BitmapFont(SDL_Renderer* renderer, const std::string& imagePath): renderer(renderer) {
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        throw std::runtime_error("Failed to load font BMP: " + std::string(IMG_GetError()));
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        throw std::runtime_error("Failed to create texture from font BMP");
    }

    // Define source rects for digits 0-9
    for (int i = 0; i < 10; ++i) {
        srcRects[i] = {i * (digitWidth), 0, digitWidth, digitHeight};
    }
}

BitmapFont::~BitmapFont() { SDL_DestroyTexture(texture); }

void BitmapFont::renderDigit(int digit, int x, int y, float scale) {
    if (digit < 0 || digit > 9)
        return;

    SDL_Rect dest = {x, y, static_cast<int>(digitWidth * scale),
                     static_cast<int>(digitHeight * scale)};

    SDL_RenderCopy(renderer, texture, &srcRects[digit], &dest);
}

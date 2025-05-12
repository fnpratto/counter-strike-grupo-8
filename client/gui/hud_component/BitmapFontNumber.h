#ifndef BITMAP_FONT_H
#define BITMAP_FONT_H

#include <string>

#include <SDL2/SDL.h>

class BitmapFont {
public:
    BitmapFont(SDL_Renderer* renderer, const std::string& imagePath);
    ~BitmapFont();

    void renderDigit(int digit, int x, int y, float scale);

private:
    SDL_Texture* texture;
    SDL_Rect srcRects[10];
    SDL_Renderer* renderer;

    const int digitWidth = 48;
    const int digitHeight = 64;
};

#endif  // BITMAP_FONT_H

#pragma once

#include <string>

#include <SDL.h>

class SDL_Texture;
class SDL_Renderer;
class SdlWindow;
class Area;

class SdlTexture {
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int width;
    int height;
    float scale;

public:
    /**
     * Crea un SDL_Texture, lanza una excepción si el filename es inválido
     **/
    SdlTexture(const std::string& filename, const SdlWindow& window);
    SdlTexture(const std::string& filename, const SdlWindow& window, int width, int height,
               float scale = 1);
    /**
     * Libera la memoria reservada por la textura
     **/
    ~SdlTexture();
    /**
     * Renderiza la textura cargada
     **/
    int render(const Area& src, const Area& dest) const;
    void render(int x, int y, double angle);
    void render(int x, int y, SDL_Rect* clip = nullptr, double angle = 0,
                SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

    SdlTexture(SdlTexture&&) = delete;
    SdlTexture& operator=(SdlTexture&&) = delete;

    SdlTexture(const SdlTexture&) = delete;
    SdlTexture& operator=(const SdlTexture&) = delete;

    SDL_Texture* getTexture() const { return texture; }

private:
    SDL_Texture* loadTexture(const std::string& filename);
};

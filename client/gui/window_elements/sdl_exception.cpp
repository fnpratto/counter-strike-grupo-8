#include "sdl_exception.h"

#include <string>

#include <SDL_image.h>

SdlException::SdlException(const char* description, const char* sdlError):
        std::exception(), description(description) {
    this->description.append("\n SDL_ERROR: ").append(sdlError);
}

const char* SdlException::what() const noexcept { return this->description.c_str(); }

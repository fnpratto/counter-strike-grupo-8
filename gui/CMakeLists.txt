cmake_minimum_required(VERSION 3.10)
project(window)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(elements
  window_elements/Area.cpp
  window_elements/SdlTexture.cpp
  window_elements/SdlException.cpp
  window_elements/SdlWindow.cpp
  window_elements/SdlText.cpp
  window_elements/Component.cpp
)

# Find SDL2
message("Agregando '${PROJECT_BINARY_DIR}' como directorio de includes")
include_directories(${PROJECT_BINARY_DIR})

add_executable(sdl-window main.cpp hud/hud_display.cpp)

target_link_libraries(sdl-window elements SDL2 SDL2_image SDL2_ttf)

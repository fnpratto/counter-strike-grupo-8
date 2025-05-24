cmake_minimum_required(VERSION 3.10)
project(elements)

add_library(elements
  window_elements/area.cpp
  window_elements/sdl_texture.cpp
  window_elements/sdl_exception.cpp
  window_elements/sdl_window.cpp
  window_elements/sdl_text.cpp
  hud_component/hud_display.cpp
  hud_component/bit_map_font_number.cpp
  shop_view/shop.cpp
  controllers/keyboardhandler.cpp
  controllers/mousehandler.cpp
  pre_game_view/list_teams.cpp
  map_view/map_view.cpp
)

target_include_directories(elements PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/window_elements
    ${CMAKE_CURRENT_SOURCE_DIR}/hud_component
)

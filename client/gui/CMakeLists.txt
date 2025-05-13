cmake_minimum_required(VERSION 3.10)
project(elements)

add_library(elements
  window_elements/Area.cpp
  window_elements/SdlTexture.cpp
  window_elements/SdlException.cpp
  window_elements/SdlWindow.cpp
  window_elements/SdlText.cpp
  hud_component/hud_display.cpp
  hud_component/BitmapFontNumber.cpp
  shop_view/shop.cpp
  controllers/keyboardhandler.cpp
  controllers/mousehandler.cpp
)

target_include_directories(elements PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/window_elements
    ${CMAKE_CURRENT_SOURCE_DIR}/hud_component
)

# Adding sources
add_subdirectory(gui)

target_sources(taller_client
    PRIVATE
        main.cpp
)

# Plain form for linking libraries
target_link_libraries(taller_client
    elements
    SDL2
    SDL2_image
    SDL2_ttf
)


target_include_directories(taller_client
   PRIVATE
        gui/window_elements
        gui/hud_component
)

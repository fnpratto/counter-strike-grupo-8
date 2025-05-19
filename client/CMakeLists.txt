# Adding sources
add_subdirectory(gui)

target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    client.cpp
    protocol.cpp
    receiver.cpp
    sender.cpp
    text_display.cpp
    qt_display.cpp
    sdl_display.cpp
    PUBLIC
    # .h files
    client.h
    display.h
    protocol.h
    receiver.h
    sender.h
    text_display.h
    qt_display.h
    sdl_display.h
    )

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

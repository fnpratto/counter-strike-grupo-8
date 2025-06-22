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
    sdl_input.cpp
    qt/create_game_window.cpp
    qt/game_list_table.cpp
    qt/lobby_window.cpp
    qt/log_in_window.cpp
    qt/resources.qrc
    connection_handler.cpp
    sound_manager.cpp

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
    sdl_input.h
    qt/create_game_window.h
    qt/game_list_table.h
    qt/lobby_window.h
    qt/log_in_window.h
    connection_handler.h
    sound_manager.h
)

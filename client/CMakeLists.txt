target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    qt/game_list_table.cpp
    qt/game_menu_window.cpp
    qt/log_in_window.cpp
    qt/resources.qrc
    PUBLIC
    # .h files
    qt/game_list_table.h
    qt/game_menu_window.h
    qt/log_in_window.h
    )

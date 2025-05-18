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

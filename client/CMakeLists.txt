target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    client.cpp
    protocol.cpp
    receiver.cpp
    sender.cpp
    text_display.cpp
    PUBLIC
    # .h files
    client.h
    display.h
    protocol.h
    receiver.h
    sender.h
    text_display.h
    )

# Define display type (text by default)
target_compile_definitions(taller_client PRIVATE CLIENT_TYPE=text)

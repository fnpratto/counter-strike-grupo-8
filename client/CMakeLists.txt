target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    client.cpp
    display.cpp
    input.cpp
    protocol.cpp
    receiver.cpp
    sender.cpp
    text_input.cpp
    PUBLIC
    # .h files
    client.h
    display.h
    input.h
    protocol.h
    receiver.h
    sender.h
    text_input.h
    )

# Define display type (text by default)
target_compile_definitions(taller_client PRIVATE CLIENT_TYPE=text)

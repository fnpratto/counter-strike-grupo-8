target_sources(taller_common
    PRIVATE
    # .cpp files
    liberror.cpp
    protocol.cpp
    resolver.cpp
    resolvererror.cpp
    socket.cpp
    PUBLIC
    # .h files
    commands.h
    game_state_update.h
    game_state.h
    liberror.h
    message.h
    models.h
    protocol.h
    queue.h
    resolver.h
    resolvererror.h
    socket.h
    thread.h
    )

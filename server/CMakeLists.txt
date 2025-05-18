target_sources(taller_server
    PRIVATE
    acceptor.cpp
    client_handler.cpp
    game.cpp
    game_thread.cpp
    lobby_monitor.cpp
    lobby_thread.cpp
    protocol.cpp
    receiver.cpp
    sender.cpp
    main.cpp
    # game
    game/game.cpp
    game/game_logic.cpp
    game/game_phase.cpp
    # player
    player/player.cpp
    player/inventory.cpp

    PUBLIC
    acceptor.h
    client_handler.h
    game.h
    game_thread.h
    lobby_monitor.h
    protocol.h
    cons.h
    receiver.h
    # clock
    clock/clock.h
    clock/mock_clock.h
    clock/real_clock.h
    # game
    game/shop.h
    game/game.h
    game/game_logic.h
    game/game_phase.h
    game/game_state.h
    game/game_state_builder.h
    # player
    player/player.h
    player/inventory.h
    # weapons
    weapons/guns_cons.h
    weapons/gun.h
    weapons/utility.h
    weapons/m3.h
    weapons/knife.h
    weapons/glock.h
    weapons/bomb.h
    weapons/awp.h
    weapons/ak47.h
    # utils
    utils/vector_2d.h
)

target_sources(taller_server
    PRIVATE
    acceptor.cpp
    client_handler.cpp
    game_thread.cpp
    lobby_monitor.cpp
    lobby_thread.cpp
    protocol.cpp
    receiver.cpp
    sender.cpp
    main.cpp
    # game
    game/game.cpp
    game/game_phase.cpp
    game/physics_system.cpp
    # map
    map/map.cpp
    # player
    player/player.cpp
    player/inventory.cpp

    PUBLIC
    acceptor.h
    client_handler.h
    game_thread.h
    lobby_monitor.h
    protocol.h
    receiver.h
    cons.h
    errors.h
    # clock
    clock/clock.h
    clock/mock_clock.h
    clock/real_clock.h
    # game
    game/shop.h
    game/game.h
    game/game_phase.h
    game/physics_system.h
    ../common/game_state.h
    # map
    map/map_builder.h
    map/map.h
    map/tile.h
    # player
    player/player.h
    player/inventory.h
    # weapons
    weapons/weapons_config.h
    weapons/gun.h
    weapons/utility.h
    weapons/m3.h
    weapons/knife.h
    weapons/glock.h
    weapons/bomb.h
    weapons/awp.h
    weapons/ak47.h
    weapons/bullet.h
    # utils
    utils/random_float_generator.h
    utils/vector_2d.h
)

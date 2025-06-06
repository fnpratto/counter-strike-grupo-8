target_sources(taller_tests
    PRIVATE
    common/test_game_state_update.cpp
    common/mock_socket.cpp
    server/map/test_map.cpp
    server/game/test_game.cpp
    server/game/test_game_phase.cpp
    server/game/test_shop.cpp
    server/player/test_player.cpp
    server/test_protocol.cpp
    # protocol
    ../server/protocol.cpp
    # game
    ../server/game/game.cpp
    ../server/game/game_phase.cpp
    ../server/game/physics_system.cpp
    # map
    ../server/map/map.cpp
    # player
    ../server/player/player.cpp
    ../server/player/inventory.cpp

    PUBLIC
    ../common/updates/state_update.h
    ../common/updates/game_update.h
    ../common/updates/phase_update.h
    ../common/updates/player_update.h
    ../common/updates/inventory_update.h
    ../common/updates/utility_update.h
    ../common/updates/gun_update.h
    ../server/states/game_state.h
    ../server/errors.h
    # clock
    ../server/clock/mock_clock.h
    # testing
    common/mock_socket.h
    # game
    ../server/game/shop.h
    ../server/game/game.h
    ../server/game/game_phase.h
    ../server/game/physics_system.h
    ../server/game/game_config.h
    # map
    ../server/map/map_builder.h
    ../server/map/map.h
    ../server/map/tile.h
    # player
    ../server/player/player.h
    ../server/player/player_config.h
    ../server/player/inventory.h
    # weapons
    ../server/weapons/weapons_config.h
    ../server/weapons/bullet.h
    ../server/weapons/gun.h
    ../server/weapons/utility.h
    ../server/weapons/m3.h
    ../server/weapons/knife.h
    ../server/weapons/glock.h
    ../server/weapons/bomb.h
    ../server/weapons/awp.h
    ../server/weapons/ak47.h
    # utils
    ../common/utils/vector_2d.h
    ../common/utils/random_float_generator.h
    )

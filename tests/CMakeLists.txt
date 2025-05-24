target_sources(taller_tests
    PRIVATE
    server/game/test_game.cpp
    server/game/test_game_phase.cpp
    server/player/test_player.cpp
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
    ../common/game_state.h
    ../server/errors.h
    # clock
    ../server/clock/mock_clock.h
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
    ../server/utils/vector_2d.h
    ../server/utils/random_float_generator.h
    )

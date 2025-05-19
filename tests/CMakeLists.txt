target_sources(taller_tests
    PRIVATE
    server/game/test_game.cpp
    server/game/test_game_phase.cpp
    server/player/test_player.cpp
    server/map/yaml_map_builder.cpp
    # game
    ../server/game/game.cpp
    ../server/game/game_logic.cpp
    ../server/game/game_phase.cpp
    # map
    ../server/map/map.cpp
    # player
    ../server/player/player.cpp
    ../server/player/inventory.cpp

    PUBLIC
    ../server/cons.h
    # clock
    ../server/clock/mock_clock.h
    # game
    ../server/game/shop.h
    ../server/game/game.h
    ../server/game/game_logic.h
    ../server/game/game_phase.h
    ../server/game/game_state.h
    ../server/game/game_state_builder.h
    # map
    ../server/map/map_builder.h
    ../server/map/map_cons.h
    ../server/map/map.h
    # player
    ../server/player/player.h
    ../server/player/inventory.h
    # weapons
    ../server/weapons/weapons_cons.h
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

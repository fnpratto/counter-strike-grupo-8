target_sources(taller_tests
    PRIVATE
    server/game/test_game.cpp
    server/game/test_player.cpp
    # game
    ../server/game/game.cpp
    ../server/game/game_state.cpp
    ../server/game/game_phase.cpp
    # player
    ../server/player/player.cpp
    ../server/player/inventory.cpp

    PUBLIC
    # clock
    ../server/clock/mock_clock.h
    # game
    ../server/game/shop.h
    ../server/game/game.h
    ../server/game/game_state.h
    ../server/game/game_phase.h
    ../server/game/game_config.h
    # player
    ../server/player/player.h
    ../server/player/inventory.h
    # weapons
    ../server/weapons/weapon.h
    ../server/weapons/m3.h
    ../server/weapons/knife.h
    ../server/weapons/gun.h
    ../server/weapons/glock.h
    ../server/weapons/bomb.h
    ../server/weapons/awp.h
    ../server/weapons/ak47.h
    )

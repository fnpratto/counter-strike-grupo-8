target_sources(taller_tests
    PRIVATE
    # .cpp files
    server/game/test_game.cpp
    server/game/test_player.cpp
    ../server/game/game.cpp
    ../server/game/player.cpp
    ../server/game/game_phase.cpp
    PUBLIC
    # .h files
    ../server/game/game.h
    ../server/game/player.h
    ../server/game/shop.h
    ../server/game/inventory.h
    ../server/game/game_phase.h
    ../server/game/clock/mock_clock.h
    )

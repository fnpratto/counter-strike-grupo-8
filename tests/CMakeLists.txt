target_sources(taller_tests
    PRIVATE
    common/test_game_state_update.cpp
    server/map/test_map.cpp
    server/game/test_game.cpp
    server/game/test_game_phase.cpp
    server/player/test_inventory.cpp
    server/player/test_player.cpp
    # game
    ../server/game/game.cpp
    ../server/game/game_phase.cpp
    ../server/game/physics_system.cpp
    # map
    ../common/map/map.cpp
    # player
    ../server/player/player.cpp
    ../server/player/inventory.cpp
    # states
    ../server/states/game_state.cpp
    ../server/states/gun_state.cpp
    ../server/states/phase_state.cpp
    ../server/states/player_state.cpp
    ../server/states/inventory_state.cpp

    PUBLIC
    ../common/updates/state_update.h
    ../common/updates/game_update.h
    ../common/updates/phase_update.h
    ../common/updates/player_update.h
    ../common/updates/inventory_update.h
    ../common/updates/gun_update.h
    ../server/errors.h
    # attack_effects
    ../server/attack_effects/attack_effect.h
    ../server/attack_effects/gun_attack.h
    ../server/attack_effects/melee_attack.h
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
    ../common/map/map.h
    ../common/map/tile.h
    # player
    ../server/player/player.h
    ../server/player/player_config.h
    ../server/player/inventory.h
    # states
    ../server/states/state.h
    ../server/states/game_state.h
    ../server/states/gun_state.h
    ../server/states/phase_state.h
    ../server/states/player_state.h
    ../server/states/inventory_state.h
    # weapons
    ../server/weapons/weapons_config.h
    ../server/weapons/gun.h
    ../server/weapons/m3.h
    ../server/weapons/knife.h
    ../server/weapons/bomb.h
    ../server/weapons/glock.h
    ../server/weapons/awp.h
    ../server/weapons/ak47.h
    # utils
    ../common/utils/vector_2d.h
    ../common/utils/random_float_generator.h
    )

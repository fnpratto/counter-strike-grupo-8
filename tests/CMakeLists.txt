target_sources(taller_tests
    PRIVATE
    common/test_game_state_update.cpp
    common/mock_socket.cpp
    server/map/test_map.cpp
    server/game/test_game.cpp
    server/game/test_game_bomb.cpp
    server/game/test_game_phase.cpp
    server/game/test_shop.cpp
    server/game/test_physics.cpp
    server/player/test_player.cpp
    server/test_protocol.cpp
    # protocol
    ../server/protocol.cpp
    ../client/protocol.cpp
    # game
    ../server/game/game.cpp
    ../server/game/game_phase.cpp
    # items
    ../server/items/gun.cpp
    ../server/items/bomb.cpp
    ../server/items/knife.cpp
    ../server/items/weapon.cpp
    ../server/items/effects/effect.cpp
    # physics
    ../server/physics/physics_system.cpp
    ../server/physics/rect_hitbox.cpp
    ../server/physics/circular_hitbox.cpp
    # map
    ../server/map/map_builder.cpp
    ../common/map/map.cpp
    # player
    ../server/player/player.cpp
    ../server/player/inventory.cpp
    ../server/player/statuses/idle_status.cpp
    ../server/player/statuses/moving_status.cpp
    ../server/player/statuses/planting_bomb_status.cpp
    ../server/player/statuses/defusing_bomb_status.cpp
    # states
    ../server/states/game_state.cpp
    ../server/states/gun_state.cpp
    ../server/states/phase_state.cpp
    ../server/states/player_state.cpp
    ../server/states/inventory_state.cpp
    ../server/states/knife_state.cpp
    ../server/states/bomb_state.cpp

    PUBLIC
    ../common/updates/state_update.h
    ../common/updates/game_update.h
    ../common/updates/phase_update.h
    ../common/updates/player_update.h
    ../common/updates/inventory_update.h
    ../common/updates/gun_update.h
    ../common/updates/knife_update.h
    ../server/errors.h
    # clock
    ../server/clock/mock_clock.h
    # testing
    common/mock_socket.h
    # game
    ../server/game/shop.h
    ../server/game/game.h
    ../server/game/game_phase.h
    ../server/game/game_config.h
    # items
    ../server/items/gun.h
    ../server/items/bomb.h
    ../server/items/knife.h
    ../server/items/weapon.h
    ../server/items/effects/effect.h
    ../server/items/effects/attack_effect.h
    ../server/items/items_config.h
    # physics
    ../server/physics/physics_system.h
    ../server/physics/target_type.h
    ../common/physics/physics_config.h
    ../server/physics/rect_hitbox.h
    ../server/physics/circular_hitbox.h
    # map
    ../server/map/map_builder.h
    ../common/map/map.h
    ../common/map/tile.h
    # player
    ../server/player/player.h
    ../server/player/player_config.h
    ../server/player/inventory.h
    ../server/player/statuses/idle_status.h
    ../server/player/statuses/moving_status.h
    ../server/player/statuses/planting_bomb_status.h
    ../server/player/statuses/player_status.h
    ../server/player/statuses/defusing_bomb_status.h
    # states
    ../server/states/state.h
    ../server/states/game_state.h
    ../server/states/gun_state.h
    ../server/states/phase_state.h
    ../server/states/player_state.h
    ../server/states/inventory_state.h
    ../server/states/knife_state.h
    ../server/states/bomb_state.h
    # utils
    ../common/utils/vector_2d.h
    ../common/utils/random_float_generator.h
    ../common/utils/rectangle.h
    ../common/utils/circle.h
    )

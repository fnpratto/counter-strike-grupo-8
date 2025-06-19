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
    # items
    items/gun.cpp
    items/bomb.cpp
    items/knife.cpp
    items/weapon.cpp
    items/effects/effect.cpp
    # physics
    physics/physics_system.cpp
    physics/rect_hitbox.cpp
    physics/circular_hitbox.cpp
    # player
    player/player.cpp
    player/inventory.cpp
    player/statuses/idle_status.cpp
    player/statuses/moving_status.cpp
    player/statuses/planting_bomb_status.cpp
    # states
    states/game_state.cpp
    states/gun_state.cpp
    states/phase_state.cpp
    states/player_state.cpp
    states/inventory_state.cpp
    states/knife_state.cpp
    states/bomb_state.cpp

    PUBLIC
    acceptor.h
    client_handler.h
    game_thread.h
    lobby_monitor.h
    protocol.h
    receiver.h
    errors.h
    # clock
    clock/clock.h
    clock/mock_clock.h
    clock/real_clock.h
    # game
    game/shop.h
    game/game.h
    game/game_phase.h
    game/game_config.h
    # items
    items/gun.h
    items/bomb.h
    items/knife.h
    items/weapon.h
    items/effects/effect.h
    items/effects/attack_effect.h
    items/items_config.h
    # physics
    physics/physics_system.h
    physics/target_type.h
    physics/rect_hitbox.h
    physics/circular_hitbox.h
    ../common/physics/physics_config.h
    # map
    map/map_builder.h
    ../common/map/map.h
    ../common/map/floor.h
    ../common/map/wall.h
    ../common/map/box.h
    # player
    player/player.h
    player/player_config.h
    player/inventory.h
    player/statuses/player_status.h
    player/statuses/idle_status.h
    player/statuses/moving_status.h
    player/statuses/planting_bomb_status.h
    # states
    states/state.h
    states/game_state.h
    states/gun_state.h
    states/phase_state.h
    states/player_state.h
    states/inventory_state.h
    states/knife_state.h
    states/bomb_state.h
)

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
    # states
    states/game_state.cpp
    states/gun_state.cpp
    states/phase_state.cpp
    states/player_state.cpp
    states/inventory_state.cpp
    states/knife_state.cpp
    states/bomb_state.cpp
    # weapons
    weapons/gun.cpp

    PUBLIC
    acceptor.h
    client_handler.h
    game_thread.h
    lobby_monitor.h
    protocol.h
    receiver.h
    errors.h
    # attack_effects
    attack_effects/attack_effect.h
    attack_effects/gun_attack.h
    attack_effects/melee_attack.h
    # clock
    clock/clock.h
    clock/mock_clock.h
    clock/real_clock.h
    # game
    game/shop.h
    game/game.h
    game/game_phase.h
    game/physics_system.h
    game/game_config.h
    # map
    map/map_builder.h
    map/map.h
    map/floor.h
    map/wall.h
    map/box.h
    # player
    player/player.h
    player/player_config.h
    player/inventory.h
    # states
    states/state.h
    states/game_state.h
    states/gun_state.h
    states/phase_state.h
    states/player_state.h
    states/inventory_state.h
    states/knife_state.h
    states/bomb_state.h
    # weapons
    weapons/weapons_config.h
    weapons/gun.h
    weapons/knife.h
    weapons/bomb.h
)

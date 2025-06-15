target_sources(taller_client
    PRIVATE
    sdl_camera.cpp
    sdl_map.cpp
    sdl_world.cpp
    sdl_player.cpp

    PUBLIC
    sdl_camera.h
    sdl_map.h
    sdl_player.h
    sdl_tile.h
    sdl_world.h
)

cmake_minimum_required(VERSION 3.20)

project(SDLToy)

# Neovim users rejoice
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.28.4
)

FetchContent_GetProperties(SDL2)

if(NOT SDL2_POPULATED)
    FetchContent_Populate(SDL2)
    add_subdirectory(${sdl2_SOURCE_DIR} ${sdl2_BINARY_DIR})
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Wall -g")
endif()

file(COPY ./assets DESTINATION ./)

add_executable(sdltoy 
  src/main.c
  src/player.c
  src/renderer_manager.c
  src/menu.c
  src/asset_loader.c
  src/game.c
  src/game_event.c
  src/game_world.c
  src/bullets.c
  src/ship.c
  src/g_session_manager.c
  src/math/vector.c
  src/math/trigonometry.c
  src/math/shapes.c
  src/math/collections.c
  src/renderer/particle.c
)

target_link_libraries(sdltoy SDL2 m)

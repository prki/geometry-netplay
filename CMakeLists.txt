cmake_minimum_required(VERSION 3.20)

project(SDLToy)

# Neovim users rejoice
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS FALSE)
set(FETCHCONTENT_QUIET FALSE)
#set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE NEVER)

include(FetchContent)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.28.4
)

set(SDL2TTF_VENDORED TRUE)
FetchContent_Declare(SDL2_ttf
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL_ttf.git"
    GIT_TAG release-2.20.1
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(SDL2 SDL2_ttf)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Wall -g")
endif()

file(COPY ./assets DESTINATION ./)

add_executable(geometry-netplay
  src/main.c
  src/player.c
  src/menu.c
  src/asset_loader.c
  src/game.c
  src/game_event.c
  src/game_world.c
  src/bullets.c
  src/ship.c
  src/g_session_manager.c
  src/f_timer.c
  src/math/vector.c
  src/math/trigonometry.c
  src/math/shapes.c
  src/math/collections.c
  src/renderer/renderer_manager.c
  src/renderer/particle.c
  src/renderer/font.c
  src/renderer/hud.c
  src/renderer/text.c
)

target_link_libraries(geometry-netplay SDL2 SDL2main SDL2_ttf m)
target_include_directories(geometry-netplay PUBLIC ${sdl2_SOURCE_DIR}/include)

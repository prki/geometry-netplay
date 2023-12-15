#include "f_controls.h"

#include <SDL.h>

F_Controls f_default_player1_controls(void) {
  F_Controls f_controls = {.move_left = SDL_SCANCODE_A,
                           .move_right = SDL_SCANCODE_D,
                           .move_up = SDL_SCANCODE_W,
                           .move_down = SDL_SCANCODE_S,
                           .shoot_left = SDL_SCANCODE_J,
                           .shoot_right = SDL_SCANCODE_L,
                           .shoot_up = SDL_SCANCODE_I,
                           .shoot_down = SDL_SCANCODE_K};

  return f_controls;
}

F_Controls f_default_player2_controls(void) {
  F_Controls f_controls = {.move_left = SDL_SCANCODE_1,
                           .move_right = SDL_SCANCODE_2,
                           .move_up = SDL_SCANCODE_3,
                           .move_down = SDL_SCANCODE_4,
                           .shoot_left = SDL_SCANCODE_5,
                           .shoot_right = SDL_SCANCODE_6,
                           .shoot_up = SDL_SCANCODE_7,
                           .shoot_down = SDL_SCANCODE_8};

  return f_controls;
}

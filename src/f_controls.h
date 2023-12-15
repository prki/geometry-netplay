#ifndef _F_CONTROLS_H_
#define _F_CONTROLS_H_

#include <SDL.h>

typedef struct {
  SDL_Scancode move_left;
  SDL_Scancode move_right;
  SDL_Scancode move_up;
  SDL_Scancode move_down;
  SDL_Scancode shoot_left;
  SDL_Scancode shoot_right;
  SDL_Scancode shoot_up;
  SDL_Scancode shoot_down;
} F_Controls;

F_Controls f_default_player1_controls(void);
F_Controls f_default_player2_controls(void);

#endif  // _F_CONTROLS_H_

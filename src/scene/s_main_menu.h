#ifndef _S_MENU_H_
#define _S_MENU_H_

#include <SDL.h>

#include "../ui/ui.h"

typedef struct {
  UI_Button* quit_button;
} S_Main_Menu;

S_Main_Menu* s_new_main_menu(SDL_Renderer* renderer);
void s_destroy_main_menu(S_Main_Menu* s_menu);
void s_render_s_main_menu(S_Main_Menu* s_menu, SDL_Renderer* renderer);

#endif  // _S_MENU_H_

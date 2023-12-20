#ifndef _S_MENU_H_
#define _S_MENU_H_

#include <SDL.h>

#include "../renderer/r_image.h"
#include "../ui/ui.h"

typedef enum {
  RETURN_UNEXPECTED = 1,
  RETURN_QUIT = 2,
  RETURN_NEW_GAME = 3,
} S_Main_Menu_Return_Codes;

typedef struct {
  UI_Button* new_game_btn;
  UI_Button* quit_btn;
  R_Image* header_image;
  R_Image* footer_image;
} S_Main_Menu;

S_Main_Menu* s_new_main_menu(SDL_Renderer* renderer);
void s_destroy_main_menu(S_Main_Menu* s_menu);
void s_render_s_main_menu(S_Main_Menu* s_menu, SDL_Renderer* renderer);

#endif  // _S_MENU_H_

#ifndef _UI_H_
#define _UI_H_
#include <SDL.h>

typedef struct {
  SDL_Rect rect;
  SDL_Texture* btn_txtr;
  SDL_Texture* btn_txtr_hover;
  void (*on_click)(void);
} UI_Button;

void ui_destroy_button(UI_Button* btn);
UI_Button* ui_new_button(int x, int y, int w, int h, SDL_Texture* txtr,
                         SDL_Texture* txtr_hover, void (*on_click)(void));

#endif  // _UI_H_

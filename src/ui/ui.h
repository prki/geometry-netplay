#ifndef _UI_H_
#define _UI_H_
#include <SDL.h>

// UI component for a button.
// Interacting with a button can either be done via a callback assigned to the
// (*on_click) function pointer, or by checking for an event via the
// ui_is_button_clicked() function, based on programmer's requirements.
typedef struct {
  SDL_Rect rect;
  SDL_Texture* btn_txtr;
  SDL_Texture* btn_txtr_hover;
  void (*on_click)(void);
} UI_Button;

void ui_destroy_button(UI_Button* btn);
UI_Button* ui_new_button(int x, int y, int w, int h, SDL_Texture* txtr,
                         SDL_Texture* txtr_hover, void (*on_click)(void));
UI_Button* ui_new_button_from_paths(int x, int y, int w, int h,
                                    const char* path, const char* path_hover,
                                    void (*on_click)(void),
                                    SDL_Renderer* renderer);
int ui_is_button_clicked(UI_Button* btn);
void ui_render_button(UI_Button* btn, SDL_Renderer* renderer);

#endif  // _UI_H_

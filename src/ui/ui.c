#include "ui.h"

#include <stdio.h>

UI_Button* ui_new_button(int x, int y, int w, int h, SDL_Texture* txtr,
                         SDL_Texture* txtr_hover, void (*on_click)(void)) {
  UI_Button* btn = NULL;
  btn = malloc(sizeof(UI_Button));
  if (btn == NULL) {
    printf("[ERROR] malloc error new ui button\n");
    return NULL;
  }

  SDL_Rect btn_rect = {.x = x, .y = y, .h = h, .w = w};
  btn->rect = btn_rect;
  btn->btn_txtr = txtr;
  btn->btn_txtr_hover = txtr_hover;
  btn->on_click = on_click;

  return btn;
}

void ui_destroy_button(UI_Button* btn) {
  if (btn != NULL) {
    if (btn->btn_txtr != NULL) {
      SDL_DestroyTexture(btn->btn_txtr);
      btn->btn_txtr = NULL;
    }
    if (btn->btn_txtr_hover != NULL) {
      SDL_DestroyTexture(btn->btn_txtr_hover);
      btn->btn_txtr_hover = NULL;
    }

    free(btn);
    btn = NULL;
  }
}

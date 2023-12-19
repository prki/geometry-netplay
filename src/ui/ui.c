#include "ui.h"

#include <SDL.h>
#include <stdio.h>

#include "../asset_loader.h"

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

UI_Button* ui_new_button_from_paths(int x, int y, int w, int h,
                                    const char* path, const char* path_hover,
                                    void (*on_click)(void),
                                    SDL_Renderer* renderer) {
  UI_Button* btn = malloc(sizeof(UI_Button));
  if (btn == NULL) {
    printf("[ERROR] malloc error new ui button from paths\n");
    return NULL;
  }

  SDL_Texture* txtr = load_texture_from_bmp(renderer, path);
  if (txtr == NULL) {
    printf("[ERROR] error new ui button from paths - base texture err\n");
    free(btn);
    return NULL;
  }

  SDL_Texture* txtr_hover = load_texture_from_bmp(renderer, path_hover);
  if (txtr_hover == NULL) {
    printf("[ERROR] error new ui button from paths - hover texture err\n");
    SDL_DestroyTexture(txtr);
    free(btn);
    return NULL;
  }

  SDL_Rect btn_rect = {.x = x, .y = y, .w = w, .h = h};
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

// Returns 0 if button is not clicked, 1 if the button is clicked.
int ui_is_button_clicked(UI_Button* btn) {
  SDL_Point point;
  int mouse_state = SDL_GetMouseState(&point.x, &point.y);
  if (!SDL_PointInRect(&point, &btn->rect)) {
    return 0;
  }

  if ((SDL_BUTTON(mouse_state) & SDL_BUTTON_LEFT) == SDL_BUTTON_LEFT) {
    return 1;
  }

  return 0;
}

void ui_render_button(UI_Button* btn, SDL_Renderer* renderer) {
  SDL_Point point;
  SDL_GetMouseState(&point.x, &point.y);
  if (SDL_PointInRect(&point, &btn->rect)) {
    SDL_RenderCopy(renderer, btn->btn_txtr_hover, NULL, &btn->rect);
  } else {
    SDL_RenderCopy(renderer, btn->btn_txtr, NULL, &btn->rect);
  }
}

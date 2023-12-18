#include "s_main_menu.h"

#include <SDL.h>
#include <stdio.h>

#include "../asset_loader.h"
#include "../renderer/renderer_manager.h"
#include "../ui/ui.h"

S_Main_Menu* s_new_main_menu(SDL_Renderer* renderer) {
  SDL_Texture* btn_txtr = NULL;
  SDL_Texture* btn_txtr_hover = NULL;

  btn_txtr = load_texture_from_bmp(renderer, "./assets/menu/quit_game.bmp");
  if (btn_txtr == NULL) {
    printf("[ERROR] err init main menu scene - loading btn_txtr fail\n");
    return NULL;
  }

  btn_txtr_hover =
      load_texture_from_bmp(renderer, "./assets/menu/quit_game_hover.bmp");
  if (btn_txtr_hover == NULL) {
    printf("[ERROR] err init main menu scene - loading btn_txtr_hover fail\n");
    SDL_DestroyTexture(btn_txtr);
    return NULL;
  }

  UI_Button* quit_btn =
      ui_new_button(50, 50, 300, 32, btn_txtr, btn_txtr_hover, NULL);
  if (quit_btn == NULL) {
    printf("[ERROR] err new main menu scene - new quit_btn fail\n");
    SDL_DestroyTexture(btn_txtr);
    SDL_DestroyTexture(btn_txtr_hover);
    return NULL;
  }

  S_Main_Menu* ret = malloc(sizeof(S_Main_Menu));
  if (ret == NULL) {
    printf("[ERROR] err new main menu scene - new main menu fail\n");
    SDL_DestroyTexture(btn_txtr);
    SDL_DestroyTexture(btn_txtr_hover);
    ui_destroy_button(quit_btn);
  }

  ret->quit_button = quit_btn;

  return ret;
}

void s_destroy_main_menu(S_Main_Menu* s_menu) {
  if (s_menu != NULL) {
    if (s_menu->quit_button != NULL) {
      ui_destroy_button(s_menu->quit_button);
    }

    free(s_menu);
  }
}

void s_render_s_main_menu(S_Main_Menu* menu, SDL_Renderer* renderer) {
  SDL_RenderCopy(renderer, menu->quit_button->btn_txtr, NULL,
                 &menu->quit_button->rect);
}

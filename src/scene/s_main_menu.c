#include "s_main_menu.h"

#include <SDL.h>
#include <stdio.h>

#include "../asset_loader.h"
#include "../ui/ui.h"

// Private function called by s_new_main_menu()
// [TODO] Same as function below, in essence - should be its own function
UI_Button* _init_quit_button(SDL_Renderer* renderer) {
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
      ui_new_button(650, 500, 300, 32, btn_txtr, btn_txtr_hover, NULL);
  if (quit_btn == NULL) {
    printf("[ERROR] err new main menu scene - new quit_btn fail\n");
    SDL_DestroyTexture(btn_txtr);
    SDL_DestroyTexture(btn_txtr_hover);
    return NULL;
  }

  return quit_btn;
}

// Private function called by s_new_main_menu()
// [TODO] Same for any button init - should be its own function
UI_Button* _init_new_game_button(SDL_Renderer* renderer) {
  SDL_Texture* btn_txtr = NULL;
  SDL_Texture* btn_txtr_hover = NULL;

  btn_txtr = load_texture_from_bmp(renderer, "./assets/menu/new_game.bmp");
  if (btn_txtr == NULL) {
    printf("[ERROR] err init new game button - loading btn_txtr fail\n");
    return NULL;
  }

  btn_txtr_hover =
      load_texture_from_bmp(renderer, "./assets/menu/new_game_hover.bmp");
  if (btn_txtr_hover == NULL) {
    printf("[ERROR] err init new game btn - loading btn_txtr_hover fail\n");
    SDL_DestroyTexture(btn_txtr);
    return NULL;
  }

  UI_Button* new_game_btn =
      ui_new_button(650, 400, 300, 32, btn_txtr, btn_txtr_hover, NULL);
  if (new_game_btn == NULL) {
    printf("[ERROR] Err new game btn - new btn fail\n");
    SDL_DestroyTexture(btn_txtr);
    SDL_DestroyTexture(btn_txtr_hover);
    return NULL;
  }

  return new_game_btn;
}

S_Main_Menu* s_new_main_menu(SDL_Renderer* renderer) {
  UI_Button* quit_btn = _init_quit_button(renderer);
  if (quit_btn == NULL) {
    return NULL;
  }

  UI_Button* new_game_btn = _init_new_game_button(renderer);
  if (new_game_btn == NULL) {
    ui_destroy_button(quit_btn);
    return NULL;
  }

  S_Main_Menu* ret = malloc(sizeof(S_Main_Menu));
  if (ret == NULL) {
    printf("[ERROR] err new main menu scene - new main menu fail\n");
    ui_destroy_button(quit_btn);
    ui_destroy_button(new_game_btn);
    return NULL;
  }

  ret->quit_btn = quit_btn;
  ret->new_game_btn = new_game_btn;

  return ret;
}

void s_destroy_main_menu(S_Main_Menu* s_menu) {
  if (s_menu != NULL) {
    if (s_menu->quit_btn != NULL) {
      ui_destroy_button(s_menu->quit_btn);
    }

    free(s_menu);
  }
}

void s_render_s_main_menu(S_Main_Menu* menu, SDL_Renderer* renderer) {
  ui_render_button(menu->quit_btn, renderer);
  ui_render_button(menu->new_game_btn, renderer);
}

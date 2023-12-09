#include "menu.h"

#include <SDL.h>
#include <stdio.h>

#include "asset_loader.h"

MenuOption* new_menu_option(SDL_Renderer* renderer, int x, int y, int h, int w,
                            const char* img_path, const char* img_hover_path) {
  SDL_Texture* btn_txtr = NULL;
  SDL_Texture* btn_txtr_hover = NULL;

  btn_txtr = load_texture_from_bmp(renderer, img_path);
  if (btn_txtr == NULL) {
    return NULL;
  }
  btn_txtr_hover = load_texture_from_bmp(renderer, img_hover_path);
  if (btn_txtr_hover == NULL) {
    return NULL;
  }

  MenuOption* menu_option = NULL;
  menu_option = malloc(sizeof(MenuOption));
  if (menu_option == NULL) {
    printf("[ERROR] malloc failed on menu option alloc\n");
    return NULL;
  }

  SDL_Rect menu_option_rect = {.x = x, .y = y, .w = w, .h = h};
  menu_option->rect = menu_option_rect;
  menu_option->box_txtr = btn_txtr;
  menu_option->box_txtr_hover = btn_txtr_hover;

  return menu_option;
}

void destroy_menu_option(MenuOption* menu_option) {
  SDL_DestroyTexture(menu_option->box_txtr);
  SDL_DestroyTexture(menu_option->box_txtr_hover);
  free(menu_option);
}

void render_menu_option(SDL_Renderer* renderer, MenuOption* menu_option) {
  SDL_Point point;
  SDL_GetMouseState(&point.x, &point.y);

  if (SDL_PointInRect(&point, &menu_option->rect)) {
    SDL_RenderCopy(renderer, menu_option->box_txtr_hover, NULL,
                   &(menu_option->rect));
  } else {
    SDL_RenderCopy(renderer, menu_option->box_txtr, NULL, &(menu_option->rect));
  }
}

int is_option_clicked(MenuOption* menu_option) {
  SDL_Point point;
  int mouse_state = SDL_GetMouseState(&point.x, &point.y);
  if (!SDL_PointInRect(&point, &(menu_option->rect))) {
    return 0;
  }

  if ((SDL_BUTTON(mouse_state) & SDL_BUTTON_LEFT) == SDL_BUTTON_LEFT) {
    return 1;
  }

  return 0;
}

void quit_game_click(void) {
  printf("Quit option has been clicked, I am a callback!\n");
}

void render_menu(SDL_Renderer* renderer, Menu* menu) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  render_menu_option(renderer, menu->quit_game_option);

  SDL_RenderPresent(renderer);
}

Menu* new_menu(SDL_Renderer* renderer) {
  Menu* menu = malloc(sizeof(Menu));
  if (menu == NULL) {
    printf("[ERROR] malloc failed on menu alloc\n");
    return NULL;
  }

  MenuOption* quit_option = new_menu_option(
      renderer, 150, 150, 32, 300, "./assets/menu/quit_game.bmp",
      "./assets/menu/quit_game_hover.bmp");
  if (quit_option == NULL) {
    free(menu);
    return NULL;
  }
  quit_option->on_click = quit_game_click;

  menu->quit_game_option = quit_option;

  return menu;
}

void destroy_menu(Menu* menu) {
  destroy_menu_option(menu->quit_game_option);
  free(menu);
}

void handle_menu_logic(Menu* menu) {
  if (is_option_clicked(menu->quit_game_option)) {
    menu->quit_game_option->on_click();
  }
}

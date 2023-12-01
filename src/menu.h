#ifndef _MENU_H_
#define _MENU_H_
#include <SDL.h>

typedef struct MenuOption {
  SDL_Rect rect;
  SDL_Texture* box_txtr;
  SDL_Texture* box_txtr_hover;
  void (*on_click)();

} MenuOption;
void destroy_menu_option(MenuOption* menu_option);
void render_menu_option(SDL_Renderer* renderer, MenuOption* menu_option);
MenuOption* new_menu_option(SDL_Renderer* renderer, int x, int y, int h, int w,
                            const char* img_path, const char* img_hover_path);

typedef struct Menu {
  MenuOption* quit_game_option;
} Menu;

Menu* new_menu(SDL_Renderer* renderer);
void destroy_menu(Menu* menu);
void handle_menu_logic(Menu* menu);
void render_menu(SDL_Renderer* renderer, Menu* menu);
#endif  // _MENU_H_

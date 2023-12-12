#include <SDL.h>
#include <stdio.h>
#include <time.h>

#include "SDL_video.h"
#include "constants.h"
#include "g_session_manager.h"
#include "game.h"
#include "renderer/renderer_manager.h"

/* [TODO] [BUG] If window succeeds but renderer does not, no sign
 * which to destroy*/
int initialize_SDL(SDL_Window** window, SDL_Renderer** renderer) {
  *window =
      SDL_CreateWindow("Pekelna parba", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  if (*window == NULL) {
    printf("[ERROR] Error creating window: %s\n", SDL_GetError());
    return 0;
  }

  *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_PRESENTVSYNC);
  if (*renderer == NULL) {
    printf("[ERROR] Error creating renderer: %s\n", SDL_GetError());
    return 0;
  }

  return 1;
}

/*void draw_menu_option(SDL_Renderer* renderer) {
  int x = 0;
  int y = 0;
  SDL_GetMouseState(&x, &y);

  SDL_Point point;
  point.x = x;
  point.y = y;

  SDL_Rect menu_option;
  menu_option.w = 300;
  menu_option.h = 32;
  menu_option.x = SCREEN_WIDTH / 2 - (menu_option.w / 2);
  menu_option.y = SCREEN_HEIGHT / 2 - menu_option.h;

  if (SDL_PointInRect(&point, &menu_option)) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  }
  SDL_RenderFillRect(renderer, &menu_option);
}

void draw_background(SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
}*/

/*int main(void) {
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;
  int keep_running = 1;
  int succ = 0;
  SDL_Event evt;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("[ERROR] SDL could not initialize. SDL Error: %s\n", SDL_GetError());
    return 1;
  }

  succ = initialize_SDL(&window, &renderer);
  if (!succ) {
    SDL_Quit();
    return 1;
  }

  Menu* menu = new_menu(renderer);
  // [TODO] renderer/window should also be freed
  if (menu == NULL) {
    SDL_Quit();
    return 1;
  }

  while (keep_running) {
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT) {
        keep_running = 0;
      }
    }
    handle_menu_logic(menu);
    render_menu(renderer, menu);
  }

  destroy_menu(menu);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
*/
int main(void) {
  int succ = 0;
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;
  RendererManager* r_mngr = NULL;
  srand(time(NULL));

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  succ = initialize_SDL(&window, &renderer);
  if (!succ) {
    SDL_Quit();
    return 1;
  } else if (window == NULL || renderer == NULL) {
    SDL_Quit();
    return 1;
  }

  r_mngr = new_renderer_manager(window, renderer);
  if (r_mngr == NULL) {
    SDL_Quit();
    return 1;
  }

  succ = r_load_assets_fonts(r_mngr);
  if (!succ) {
    SDL_Quit();
    destroy_renderer_manager(r_mngr);
    return 1;
  }

  Game* game = initialize_game();
  if (game == NULL) {
    SDL_Quit();
    return 1;
  }
  // [TODO] Shouldn't this also clean up game resources and such?
  succ = r_initialize_hud(r_mngr->hud, r_mngr->renderer, game->player);
  if (!succ) {
    SDL_Quit();
    return 1;
  }

  succ = register_game(r_mngr, game);
  if (!succ) {
    SDL_Quit();
    return 1;
  }

  G_Session_Manager g_sess_mgr;
  initialize_game_session(&g_sess_mgr, game, r_mngr);
  add_ai_player_to_session(&g_sess_mgr);

  int keep_running = 1;
  SDL_Event evt;
  while (keep_running) {
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT) {
        keep_running = 0;
      }
    }
    game_update(game);

    render_frame(r_mngr);
  }

  destroy_game(game);
  destroy_renderer_manager(r_mngr);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

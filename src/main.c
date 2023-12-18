#include <SDL.h>
#include <stdio.h>
#include <time.h>

#include "SDL_video.h"
#include "constants.h"
#include "f_timer.h"
#include "g_session_manager.h"
#include "game.h"
#include "renderer/renderer_manager.h"
#include "scene/s_main_menu.h"
#include "scene/s_orchestrator.h"

// This game has been programmed to assume VSYNC is on - however, physics
// and rendering are calculated using elapsed delta times. As such, it is
// possible to unlock the framerate. HOWEVER, the calculation is not precise
// in any way - the calculation uses SDL_Delay() to sleep the elapsed time
// until a new frame may start. This is hardly ideal as SDL_Delay() uses OS
// sleep AND the precision goes up to miliseconds (hence anything above 1000FPS
// would never be seen). In order to do this more precisely, OS-level timer
// would need to be introduced (see c++ std::chrono).
// Hence, these switches serve to provide testing of framerate-independency
// with different monitors (60/144/...Hz), but at the end of the day, they
// should be compiled to run with VSYNC ON.
#define VSYNC_ON 0
#define MAX_FPS 200

/* [TODO] [BUG] If window succeeds but renderer does not, no sign
 * which to destroy*/
int initialize_SDL(SDL_Window** window, SDL_Renderer** renderer, F_Config cfg) {
  *window =
      SDL_CreateWindow("Pekelna parba", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  if (*window == NULL) {
    printf("[ERROR] Error creating window: %s\n", SDL_GetError());
    return 0;
  }

  if (cfg.r_vsync) {
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_PRESENTVSYNC);
  } else {
    *renderer = SDL_CreateRenderer(*window, -1, 0);
  }
  if (*renderer == NULL) {
    printf("[ERROR] Error creating renderer: %s\n", SDL_GetError());
    return 0;
  }

  return 1;
}

int main(void) {
  int succ = 0;
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;
  RendererManager* r_mngr = NULL;
  srand(time(NULL));
  F_Config cfg = {.r_vsync = !VSYNC_ON, .r_max_fps = MAX_FPS};

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  succ = initialize_SDL(&window, &renderer, cfg);
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

  F_Timer f_timer;
  f_timer_init(&f_timer);

  S_Orchestrator* s_orchestrator = s_new_orchestrator();
  if (s_orchestrator == NULL) {
    SDL_Quit();
    return 1;
  }

  S_Main_Menu* s_main_menu = s_new_main_menu(r_mngr->renderer);
  if (s_main_menu == NULL) {
    SDL_Quit();
    return 1;
  }

  succ = s_orchestrator_register_r_mngr(s_orchestrator, r_mngr);
  if (!succ) {
    SDL_Quit();
    return 1;
  }
  succ = s_orchestrator_register_s_main_menu(s_orchestrator, s_main_menu);

  int ret = s_run_main_menu_loop(s_orchestrator);
  if (ret == RETURN_QUIT) {
    SDL_Quit();
    return 1;
  }

  G_Session_Manager g_sess_mgr;
  initialize_game_session(&g_sess_mgr, game, r_mngr);
  // [TODO] Validate return code/error
  setup_game_session(&g_sess_mgr, G_GAMETYPE_LOCAL_MULTIPLAYER, 2);

  succ = register_game(r_mngr, game);
  if (!succ) {
    SDL_Quit();
    return 1;
  }

  // [TODO] Validate return code/error
  run_game_session(&g_sess_mgr, &cfg);

  destroy_game(game);
  s_destroy_orchestrator(s_orchestrator);
  destroy_renderer_manager(r_mngr);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

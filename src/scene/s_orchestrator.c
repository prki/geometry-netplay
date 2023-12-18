#include "s_orchestrator.h"

#include <stdio.h>

#include "../renderer/renderer_manager.h"

S_Orchestrator* s_new_orchestrator(void) {
  S_Orchestrator* ret = malloc(sizeof(S_Orchestrator));
  if (ret == NULL) {
    printf("[ERROR] err new s_orchestrator - malloc NULL\n");
    return NULL;
  }

  ret->s_main_menu = NULL;
  ret->r_mngr = NULL;
  ret->s_results = NULL;

  return ret;
}

void s_destroy_orchestrator(S_Orchestrator* s_orche) {
  if (s_orche != NULL) {
    if (s_orche->s_main_menu != NULL) {
      s_destroy_main_menu(s_orche->s_main_menu);
      s_orche->s_main_menu = NULL;
    }
    if (s_orche->s_results != NULL) {
      s_destroy_results(s_orche->s_results);
      s_orche->s_results = NULL;
    }

    free(s_orche);
    s_orche = NULL;
  }
}

int s_orchestrator_register_r_mngr(S_Orchestrator* s_orche,
                                   RendererManager* r_mngr) {
  if (s_orche == NULL) {
    printf("[ERROR] Err registering r_mngr to s_orche - s_orche NULL\n");
    return 0;
  }
  if (r_mngr == NULL) {
    printf("[ERROR] Err registering r_mngr to s_orche - r_mngr NULL\n");
    return 0;
  }

  s_orche->r_mngr = r_mngr;

  return 1;
}

int s_orchestrator_register_s_main_menu(S_Orchestrator* s_orche,
                                        S_Main_Menu* s_main_menu) {
  if (s_orche == NULL) {
    printf("[ERROR] Err registering s_main_menu to s_orche - s_orche NULL\n");
    return 0;
  }
  if (s_main_menu == NULL) {
    printf("[ERROR] Err registering s_main_menu s_orche - s_main_menu NULL\n");
    return 0;
  }

  s_orche->s_main_menu = s_main_menu;

  return 1;
}

int s_orchestrator_register_s_results(S_Orchestrator* s_orche,
                                      S_Results* s_results) {
  if (s_orche == NULL) {
    printf("[ERROR] Err registering s_results to s_orche - s_orche NULL\n");
    return 0;
  }
  if (s_results == NULL) {
    printf("[ERROR] Err registering s_results to s_orche - s_results NULL\n");
    return 0;
  }

  s_orche->s_results = s_results;

  return 1;
}

// [TODO] All the "run loop" functions are going to work in a similar way - is
// there any way to make them into a generic thing?
int s_run_main_menu_loop(S_Orchestrator* s_orche) {
  int keep_running = 1;
  SDL_Event evt;
  while (keep_running) {
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT) {
        return 2;
      }
    }

    r_render_s_main_menu(s_orche->r_mngr, s_orche->s_main_menu);
    if (ui_is_button_clicked(s_orche->s_main_menu->quit_btn)) {
      return RETURN_QUIT;
    }
    if (ui_is_button_clicked(s_orche->s_main_menu->new_game_btn)) {
      return RETURN_NEW_GAME;
    }
    r_display_frame(s_orche->r_mngr);
  }

  return 1;
}

int s_run_results_loop(S_Orchestrator* s_orche) {
  int keep_running = 1;
  SDL_Event evt;
  while (keep_running) {
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT) {
        return 2;
      }
    }

    r_render_s_results(s_orche->r_mngr, s_orche->s_results);
    r_display_frame(s_orche->r_mngr);
  }

  return 1;
}

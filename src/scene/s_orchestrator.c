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

  return ret;
}

void s_destroy_orchestrator(S_Orchestrator* s_orche) {
  if (s_orche != NULL) {
    if (s_orche->s_main_menu != NULL) {
      s_destroy_main_menu(s_orche->s_main_menu);
      s_orche->s_main_menu = NULL;
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

// [TODO] Return codes should be as #define constants or an ENUM
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
    r_display_frame(s_orche->r_mngr);
  }

  return 1;
}

#ifndef _S_ORCHESTRATOR_
#define _S_ORCHESTRATOR_

#include "../renderer/renderer_manager.h"
#include "s_main_menu.h"

typedef struct {
  S_Main_Menu* s_main_menu;
  RendererManager* r_mngr;  // Not owned when s_orchestrator free'd
} S_Orchestrator;

S_Orchestrator* s_new_orchestrator(void);
void s_destroy_orchestrator(S_Orchestrator* s_orche);
int s_orchestrator_register_r_mngr(S_Orchestrator* s_orche,
                                   RendererManager* r_mngr);
int s_orchestrator_register_s_main_menu(S_Orchestrator* s_orche,
                                        S_Main_Menu* s_main_menu);
int s_run_main_menu_loop(S_Orchestrator* s_orche);

#endif  // _S_ORCHESTRATOR_

#ifndef _S_ORCHESTRATOR_
#define _S_ORCHESTRATOR_

#include "../renderer/renderer_manager.h"
#include "s_main_menu.h"

// Scene orchestrator is the highest level layer in the game's architecture. It
// is responsible for containing all of the scenes/views visible to the player -
// such as the menus, gameplay scene and/or others.
// Each scene is provided as its own member and a function for orchestrator is
// provided which runs the entire loop with all its possible
// dependencies/specifics.
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

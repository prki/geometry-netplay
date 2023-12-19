#ifndef _S_ORCHESTRATOR_
#define _S_ORCHESTRATOR_

#include "../renderer/renderer_manager.h"
#include "s_game.h"
#include "s_main_menu.h"
#include "s_results.h"

typedef enum {
  SCENE_MAIN_MENU,
  SCENE_GAME,
  SCENE_RESULTS,
  SCENE_QUIT,
} S_Scene_Code;

// Scene orchestrator is the highest level layer in the game's architecture. It
// is responsible for containing all of the scenes/views visible to the player -
// such as the menus, gameplay scene and/or others.
// Each scene is provided as its own member and a function for orchestrator is
// provided which runs the entire loop with all its possible
// dependencies/specifics.
typedef struct {
  S_Main_Menu* s_main_menu;
  S_Results* s_results;
  S_Game* s_game;
  RendererManager* r_mngr;  // Not owned when s_orchestrator free'd
} S_Orchestrator;

S_Orchestrator* s_new_orchestrator(void);
void s_destroy_orchestrator(S_Orchestrator* s_orche);
int s_orchestrator_load_all_scenes(S_Orchestrator* s_orche);
int s_orchestrator_register_r_mngr(S_Orchestrator* s_orche,
                                   RendererManager* r_mngr);
int s_orchestrator_register_s_game(S_Orchestrator* s_orche, S_Game* s_game);
int s_orchestrator_register_s_main_menu(S_Orchestrator* s_orche,
                                        S_Main_Menu* s_main_menu);
int s_orchestrator_register_s_results(S_Orchestrator* s_orche,
                                      S_Results* s_results);
ResultReport s_calc_result_report(S_Game* s_game);
int s_run_main_menu_loop(S_Orchestrator* s_orche);
S_Scene_Code s_run_results_loop(S_Orchestrator* s_orche);
int s_run_game_loop(S_Orchestrator* s_orche);
void s_orchestrate_scenes(S_Orchestrator* s_orche);

#endif  // _S_ORCHESTRATOR_

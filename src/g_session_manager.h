#ifndef _G_SESSION_MANAGER_H_
#define _G_SESSION_MANAGER_H_
#include "f_config.h"
#include "game.h"
#include "renderer/renderer_manager.h"

#define G_GAMETYPE_LOCAL_MULTIPLAYER 1
#define G_GAMETYPE_LOCAL_SINGLEPLAYER 2

typedef struct {
  unsigned int timelimit;  // in seconds
} G_Game_Rules;

typedef struct {
  unsigned int time_elapsed;  // milliseconds
} G_Rule_Checker;

int is_game_over(G_Game_Rules g_rules, G_Rule_Checker g_rule_checker);

typedef struct G_Session_Manager {
  Game* game;
  RendererManager* r_mngr;
  F_Timer f_timer;
  G_Game_Rules g_rules;
  G_Rule_Checker g_rule_checker;
} G_Session_Manager;

void initialize_game_session(G_Session_Manager* g_sess_mgr, Game* game,
                             RendererManager* r_mngr);
void add_ai_player_to_session(G_Session_Manager* g_sess_mgr);
int setup_game_session(G_Session_Manager* g_sess_mgr, int gametype,
                       unsigned int timelimit);
int run_game_session(G_Session_Manager* g_sess_mgr, F_Config* f_cfg);

#endif  // _G_SESSION_MANAGER_H_

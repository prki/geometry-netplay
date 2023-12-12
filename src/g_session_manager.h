#ifndef _G_SESSION_MANAGER_H_
#include "game.h"
#include "renderer/renderer_manager.h"

typedef struct G_Session_Manager {
  Game* game;
  RendererManager* r_mngr;
} G_Session_Manager;

void initialize_game_session(G_Session_Manager* g_sess_mgr, Game* game,
                             RendererManager* r_mngr);
void add_ai_player_to_session(G_Session_Manager* g_sess_mgr);

#define _G_SESSION_MANAGER_H_
#endif  // _G_SESSION_MANAGER_H_

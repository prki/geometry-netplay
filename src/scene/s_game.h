#ifndef _S_GAME_H_
#define _S_GAME_H_

#include "../audio/a_sound.h"
#include "../g_session_manager.h"

typedef struct {
  G_Session_Manager g_sess_mgr;
  Game* game;
  A_Sound* music;
} S_Game;

S_Game* s_new_game(RendererManager* r_mngr);
void s_destroy_game(S_Game* s_game);

#endif  // _S_GAME_H_

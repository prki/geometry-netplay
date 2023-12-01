#include "g_session_manager.h"

#include "game.h"
#include "math/vector.h"

void initialize_game_session(G_Session_Manager* g_sess_mgr, Game* game,
                             RendererManager* r_mngr) {
  if (game == NULL) {
    printf("[ERROR] err initializing game session - game NULL\n");
    return;
  }
  if (r_mngr == NULL) {
    printf("[ERROR] err initializing game session - r_mngr NULL\n");
    return;
  }

  g_sess_mgr->game = game;
  g_sess_mgr->r_mngr = r_mngr;
}

void add_ai_player_to_session(G_Session_Manager* g_sess_mgr) {
  Player* ai_player;

  ai_player = add_enemy_player_ai(g_sess_mgr->game, NULL, 4, 32,
                                  (vec2d){.x = 1, .y = 0});
  if (ai_player == NULL) {
    printf("[ERROR] g_sess_mgr unable to add ai player\n");
    return;
  }

  register_player(g_sess_mgr->r_mngr, ai_player);
}

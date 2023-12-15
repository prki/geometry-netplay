#include "g_session_manager.h"

#include <stdio.h>

#include "f_controls.h"
#include "game.h"
#include "math/vector.h"
#include "player.h"

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

  ai_player = add_enemy_player_ai(g_sess_mgr->game, NULL, 8, 32,
                                  (vec2d){.x = 1, .y = 0});
  if (ai_player == NULL) {
    printf("[ERROR] g_sess_mgr unable to add ai player\n");
    return;
  }

  register_player(g_sess_mgr->r_mngr, ai_player);
}

int _setup_local_multiplayer(G_Session_Manager* g_sess_mgr) {
  F_Controls controls_p1 = f_default_player1_controls();
  vec2d p1_initial_ship_pos = {.x = 150, .y = 416};
  double hurtcirc_radius = 8;
  size_t ship_width = 32;
  vec2d shoot_direction = {.x = 0, .y = -1};
  Player* p1 = new_pc_player(p1_initial_ship_pos, hurtcirc_radius, ship_width,
                             shoot_direction, controls_p1);
  if (p1 == NULL) {
    printf("[ERROR] Error setting up local multiplayer - p1 null\n");
    return 0;
  }
  F_Controls controls_p2 = f_default_player2_controls();
  vec2d p2_initial_ship_pos = {.x = 1450, .y = 416};
  Player* p2 = new_pc_player(p2_initial_ship_pos, hurtcirc_radius, ship_width,
                             shoot_direction, controls_p2);
  if (p2 == NULL) {
    printf("[ERROR] Error setting up local multiplayer - p2 null\n");
    return 0;
  }

  g_sess_mgr->game->players[0] = p1;
  g_sess_mgr->game->players[1] = p2;

  return 1;
}

int _setup_local_singleplayer(G_Session_Manager* g_sess_mgr) {
  F_Controls controls_p1 = f_default_player1_controls();
  vec2d p1_initial_ship_pos = {.x = 150, .y = 416};
  double hurtcirc_radius = 8;
  size_t ship_width = 32;
  vec2d shoot_direction = {.x = 0, .y = -1};
  Player* player = new_pc_player(p1_initial_ship_pos, hurtcirc_radius,
                                 ship_width, shoot_direction, controls_p1);
  if (player == NULL) {
    printf("[ERROR] Error setting up local singleplayer - player null\n");
    return 0;
  }

  g_sess_mgr->game->players[0] = player;

  return 1;
}

// Possible gametypes - G_GAMETYPE_* - see g_session_manager.h.
int setup_game_session(G_Session_Manager* g_sess_mgr, int gametype) {
  int succ = 1;
  if (gametype == G_GAMETYPE_LOCAL_MULTIPLAYER) {
    succ = _setup_local_multiplayer(g_sess_mgr);
  } else if (gametype == G_GAMETYPE_LOCAL_SINGLEPLAYER) {
    succ = _setup_local_singleplayer(g_sess_mgr);
  } else {
    printf("[ERROR] Attempted to set up game session with unknown gametype\n");
    succ = 0;
  }

  return succ;
}

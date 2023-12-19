#include "s_game.h"

#include <stdio.h>
#include <stdlib.h>

#include "../renderer/renderer_manager.h"

S_Game* s_new_game(RendererManager* r_mngr) {
  S_Game* s_game = malloc(sizeof(S_Game));
  if (s_game == NULL) {
    printf("[ERROR] Err s_new_game - s_game malloc null\n");
    return NULL;
  }
  s_game->game = NULL;

  Game* game = initialize_game();
  if (game == NULL) {
    printf("[ERROR] Err s_new_game - game init fail\n");
    free(s_game);
    return NULL;
  }

  s_game->game = game;

  initialize_game_session(&s_game->g_sess_mgr, s_game->game, r_mngr);

  return s_game;
}

void s_destroy_game(S_Game* s_game) {
  if (s_game != NULL) {
    if (s_game->game != NULL) {
      destroy_game(s_game->game);
    }
    free(s_game);
  }
}

#include "g_session_manager.h"

#include <SDL.h>
#include <math.h>
#include <stdio.h>

#include "SDL_timer.h"
#include "f_config.h"
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

  f_timer_init(&g_sess_mgr->f_timer);
  g_sess_mgr->game = game;
  g_sess_mgr->r_mngr = r_mngr;
  g_sess_mgr->g_rules.timelimit = 0;
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
  double hurtcirc_radius = 12;
  size_t ship_width = 32;
  vec2d shoot_direction = {.x = 0, .y = -1};
  Player* p1 = new_pc_player(p1_initial_ship_pos, hurtcirc_radius, ship_width,
                             shoot_direction, controls_p1, RED);
  if (p1 == NULL) {
    printf("[ERROR] Error setting up local multiplayer - p1 null\n");
    return 0;
  }
  F_Controls controls_p2 = f_default_player2_controls();
  vec2d p2_initial_ship_pos = {.x = 1450, .y = 416};
  Player* p2 = new_pc_player(p2_initial_ship_pos, hurtcirc_radius, ship_width,
                             shoot_direction, controls_p2, BLUE);
  if (p2 == NULL) {
    printf("[ERROR] Error setting up local multiplayer - p2 null\n");
    return 0;
  }

  g_sess_mgr->game->players[0] = p1;
  g_sess_mgr->game->players[1] = p2;

  int succ = r_initialize_hud(
      g_sess_mgr->r_mngr->hud, g_sess_mgr->r_mngr->renderer,
      (const Player**)g_sess_mgr->game->players, &g_sess_mgr->f_timer);
  if (!succ) {
    printf(
        "[ERROR] Error setting up local multiplayer - HUD initialization "
        "failure\n");
    return 0;
  }

  return 1;
}

int _setup_local_singleplayer(G_Session_Manager* g_sess_mgr) {
  F_Controls controls_p1 = f_default_player1_controls();
  vec2d p1_initial_ship_pos = {.x = 150, .y = 416};
  double hurtcirc_radius = 8;
  size_t ship_width = 32;
  vec2d shoot_direction = {.x = 0, .y = -1};
  Player* player = new_pc_player(p1_initial_ship_pos, hurtcirc_radius,
                                 ship_width, shoot_direction, controls_p1, RED);
  if (player == NULL) {
    printf("[ERROR] Error setting up local singleplayer - player null\n");
    return 0;
  }

  g_sess_mgr->game->players[0] = player;

  return 1;
}

// Possible gametypes - G_GAMETYPE_* - see g_session_manager.h.
// In case no timelimit should be set, use 0.
int setup_game_session(G_Session_Manager* g_sess_mgr, int gametype,
                       unsigned int timelimit) {
  int succ = 1;
  if (gametype == G_GAMETYPE_LOCAL_MULTIPLAYER) {
    succ = _setup_local_multiplayer(g_sess_mgr);
  } else if (gametype == G_GAMETYPE_LOCAL_SINGLEPLAYER) {
    succ = _setup_local_singleplayer(g_sess_mgr);
  } else {
    printf("[ERROR] Attempted to set up game session with unknown gametype\n");
    return 0;
  }

  g_sess_mgr->g_rules.timelimit = timelimit;
  // [TODO] Workaround cleaning up bullet pool when rematching. This should be
  // handled by the scene once game becomes a scene
  for (size_t i = 0; i < g_sess_mgr->game->bullet_pool->pool_size; i++) {
    Bullet* tmp = &g_sess_mgr->game->bullet_pool->bullets[i];
    disable_bullet(tmp);
  }

  return succ;
}

int is_game_over(G_Game_Rules g_rules, G_Rule_Checker g_rule_checker) {
  if (g_rules.timelimit != 0) {
    double time_elapsed_secs = g_rule_checker.time_elapsed / 1000.0f;
    if (time_elapsed_secs >= g_rules.timelimit) {
      return 1;
    }
  }

  return 0;
}

int is_game_draw(const Game* game) {
  int score_max = 0;
  for (size_t i = 0; i < G_MAX_PC_PLAYERS; i++) {
    const Player* plr_tmp = game->players[i];
    if (plr_tmp != NULL) {
      if (plr_tmp->score > score_max) {
        score_max = plr_tmp->score;
      }
    }
  }

  int score_max_counter = 0;
  for (size_t i = 0; i < G_MAX_PC_PLAYERS; i++) {
    const Player* plr_tmp = game->players[i];
    if (plr_tmp != NULL) {
      if (plr_tmp->score == score_max) {
        score_max_counter += 1;
      }
    }
  }

  if (score_max_counter == 1) {
    return 0;
  }
  return 1;
}

// Function running the main game loop - updating the game each tick and
// rendering it each tick. In essence, this is the standard "game loop wrapper"
// common in the main() function. Note that this function is blocking - once
// it's called, it will run until the window is either closed or the game is
// finished based on its set rules.
// [TODO] Implement enum/defines with return codes (e.g. game quit, game over ->
// show result screen, ...)
// [TODO] Validate that timer really starts at 00:00 msec?
int run_game_session(G_Session_Manager* g_sess_mgr, F_Config* f_cfg) {
  int keep_running = 1;
  SDL_Event evt;
  float max_delta = (1.0 / f_cfg->r_max_fps) * 1000.0;
  g_sess_mgr->g_rule_checker.time_elapsed = 0;
  f_timer_init(&g_sess_mgr->f_timer);

  while (keep_running) {
    Uint64 start_time = SDL_GetPerformanceCounter();
    if (is_game_over(g_sess_mgr->g_rules, g_sess_mgr->g_rule_checker)) {
      if (!is_game_draw(g_sess_mgr->game)) {
        return 2;
      }
    }

    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT) {
        keep_running = 0;
      }
    }

    f_timer_update(&g_sess_mgr->f_timer);
    g_sess_mgr->g_rule_checker.time_elapsed += g_sess_mgr->f_timer.delta_time;
    double game_delta = g_sess_mgr->f_timer.delta_time / 1000.0f;

    game_update(g_sess_mgr->game, game_delta);
    render_frame(g_sess_mgr->r_mngr, game_delta);
    r_update_hud(g_sess_mgr->r_mngr->hud, g_sess_mgr->r_mngr->renderer);
    r_render_hud(g_sess_mgr->r_mngr->hud, g_sess_mgr->r_mngr->renderer,
                 g_sess_mgr->g_rule_checker.time_elapsed);
    r_display_frame(g_sess_mgr->r_mngr);

    Uint64 end_time = SDL_GetPerformanceCounter();
    if (!f_cfg->r_vsync) {
      float elapsed_ms = (end_time - start_time) /
                         (float)SDL_GetPerformanceFrequency() * 1000.0f;
      SDL_Delay(floor(max_delta - elapsed_ms));
    }
  }

  return 1;
}

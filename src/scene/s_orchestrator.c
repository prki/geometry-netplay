#include "s_orchestrator.h"

#include <stdio.h>

#include "../f_config.h"
#include "../g_session_manager.h"
#include "../renderer/renderer_manager.h"

S_Orchestrator* s_new_orchestrator(void) {
  S_Orchestrator* ret = malloc(sizeof(S_Orchestrator));
  if (ret == NULL) {
    printf("[ERROR] err new s_orchestrator - malloc NULL\n");
    return NULL;
  }

  ret->s_main_menu = NULL;
  ret->r_mngr = NULL;
  ret->s_results = NULL;
  ret->s_game = NULL;

  return ret;
}

void s_destroy_orchestrator(S_Orchestrator* s_orche) {
  if (s_orche != NULL) {
    if (s_orche->s_main_menu != NULL) {
      s_destroy_main_menu(s_orche->s_main_menu);
      s_orche->s_main_menu = NULL;
    }
    if (s_orche->s_results != NULL) {
      s_destroy_results(s_orche->s_results);
      s_orche->s_results = NULL;
    }
    if (s_orche->s_game != NULL) {
      s_destroy_game(s_orche->s_game);
      s_orche->s_game = NULL;
    }

    free(s_orche);
    s_orche = NULL;
  }
}

// Function initializing all underlying scenes and registering them.
// This is the function which should be used publicly, unless there is a
// reason why only some scenes should be loaded. In our case, as the
// footprint is tiny, we can just load all scenes and perform context switching
// afterwards without any operations afterwards.
// It is assumed the parameter s_orche (S_Orchestrator*) has r_mngr assigned, as
// other scenes require access to SDL_Renderer*
// Function does not clean up resources if any initialization fails, but its
// state likely requires calling s_destroy_orchestrator(), since running the
// game does not make sense at that point.
// Returns 1 on success, 0 on failure.
int s_orchestrator_load_all_scenes(S_Orchestrator* s_orche) {
  if (s_orche->r_mngr == NULL) {
    printf("[ERROR] Err loading scenes in s_orchestrator - r_mngr NULL\n");
    return 0;
  }

  S_Main_Menu* s_main_menu = s_new_main_menu(s_orche->r_mngr->renderer);
  if (s_main_menu == NULL) {
    printf("[ERROR] Err loading scenes in s_orchestrator - s_main_menu NULL\n");
    return 0;
  }
  s_orchestrator_register_s_main_menu(s_orche, s_main_menu);

  S_Results* s_results =
      s_new_results(s_orche->r_mngr->font_storage, s_orche->r_mngr->renderer);
  if (s_results == NULL) {
    printf("[ERROR] Err loading scenes in s_orchestrator - s_results NULL\n");
    return 0;
  }
  s_orchestrator_register_s_results(s_orche, s_results);

  S_Game* s_game = s_new_game(s_orche->r_mngr);
  if (s_game == NULL) {
    printf("[ERROR] Err loading scenes in s_orchestrator - s_game NULL\n");
    return 0;
  }
  s_orchestrator_register_s_game(s_orche, s_game);

  S_Boot* s_boot = s_new_boot(s_orche->r_mngr->renderer);
  if (s_boot == NULL) {
    printf("[ERROR] Err loading scenes in s_orchestrator - s_boot NULL\n");
    return 0;
  }
  s_orchestrator_register_s_boot(s_orche, s_boot);

  return 1;
}

int s_orchestrator_register_r_mngr(S_Orchestrator* s_orche,
                                   RendererManager* r_mngr) {
  if (s_orche == NULL) {
    printf("[ERROR] Err registering r_mngr to s_orche - s_orche NULL\n");
    return 0;
  }
  if (r_mngr == NULL) {
    printf("[ERROR] Err registering r_mngr to s_orche - r_mngr NULL\n");
    return 0;
  }

  s_orche->r_mngr = r_mngr;

  return 1;
}

int s_orchestrator_register_s_boot(S_Orchestrator* s_orche, S_Boot* s_boot) {
  if (s_orche == NULL) {
    printf("[ERROR] Err registering s_boot to s_orche - s_orche NULL\n");
    return 0;
  }
  if (s_boot == NULL) {
    printf("[ERROR] Err registering s_boot to s_orche - s_boot NULL\n");
    return 0;
  }

  s_orche->s_boot = s_boot;

  return 1;
}

int s_orchestrator_register_s_game(S_Orchestrator* s_orche, S_Game* s_game) {
  if (s_orche == NULL) {
    printf("[ERROR] Err registering s_game to s_orche - s_orche NULL\n");
    return 0;
  }
  if (s_game == NULL) {
    printf("[ERROR] Err registering s_game to s_orche - s_game NULL\n");
    return 0;
  }

  s_orche->s_game = s_game;

  return 1;
}

int s_orchestrator_register_s_main_menu(S_Orchestrator* s_orche,
                                        S_Main_Menu* s_main_menu) {
  if (s_orche == NULL) {
    printf("[ERROR] Err registering s_main_menu to s_orche - s_orche NULL\n");
    return 0;
  }
  if (s_main_menu == NULL) {
    printf("[ERROR] Err registering s_main_menu s_orche - s_main_menu NULL\n");
    return 0;
  }

  s_orche->s_main_menu = s_main_menu;

  return 1;
}

int s_orchestrator_register_s_results(S_Orchestrator* s_orche,
                                      S_Results* s_results) {
  if (s_orche == NULL) {
    printf("[ERROR] Err registering s_results to s_orche - s_orche NULL\n");
    return 0;
  }
  if (s_results == NULL) {
    printf("[ERROR] Err registering s_results to s_orche - s_results NULL\n");
    return 0;
  }

  s_orche->s_results = s_results;

  return 1;
}

// [TODO] Implementation only considers 2 players. Make this more universal.
ResultReport s_calc_result_report(S_Orchestrator* s_orche) {
  S_Game* s_game = s_orche->s_game;
  int scores[2];
  for (size_t i = 0; i < 2; i++) {
    scores[i] = 0;
  }

  const Player* tmp = NULL;
  for (size_t i = 0; i < 2; i++) {
    tmp = s_game->game->players[i];
    if (tmp == NULL) {
      printf("[WARN] Player was NULL when calculating result report\n");
    }
    scores[i] = tmp->score;
  }

  if (scores[0] > scores[1]) {
    return (ResultReport){.score_winner = scores[0],
                          .score_loser = scores[1],
                          .winner_idx = 0,
                          .loser_idx = 1};
  } else {
    return (ResultReport){.score_winner = scores[1],
                          .score_loser = scores[0],
                          .winner_idx = 1,
                          .loser_idx = 0};
  }
}

// [TODO] All the "run loop" functions are going to work in a similar way - is
// there any way to make them into a generic thing?
int s_run_main_menu_loop(S_Orchestrator* s_orche) {
  int keep_running = 1;
  SDL_Event evt;
  while (keep_running) {
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT) {
        return 2;
      }
    }

    r_render_s_main_menu(s_orche->r_mngr, s_orche->s_main_menu);
    if (ui_is_button_clicked(s_orche->s_main_menu->quit_btn)) {
      return RETURN_QUIT;
    }
    if (ui_is_button_clicked(s_orche->s_main_menu->new_game_btn)) {
      return RETURN_NEW_GAME;
    }
    r_display_frame(s_orche->r_mngr);
  }

  return 1;
}

// [TODO] All the "run loop" functions are going to work in a similar way - is
// there any way to make them into a generic thing?
S_Scene_Code s_run_results_loop(S_Orchestrator* s_orche) {
  int keep_running = 1;
  SDL_Event evt;
  while (keep_running) {
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT) {
        return SCENE_QUIT;
      }
    }

    if (ui_is_button_clicked(s_orche->s_results->rematch_btn)) {
      return SCENE_GAME;
    }
    if (ui_is_button_clicked(s_orche->s_results->menu_btn)) {
      return SCENE_MAIN_MENU;
    }

    r_render_s_results(s_orche->r_mngr, s_orche->s_results);
    r_display_frame(s_orche->r_mngr);
  }

  return SCENE_QUIT;
}

S_Scene_Code s_run_boot_loop(S_Orchestrator* s_orche) {
  int keep_running = 1;
  SDL_Event evt;
  F_Timer f_timer;
  f_timer_init(&f_timer);
  int elapsed_time = 0;
  while (keep_running) {
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT) {
        return SCENE_QUIT;
      }
    }

    SDL_RenderClear(s_orche->r_mngr->renderer);
    s_render_s_boot(s_orche->s_boot, s_orche->r_mngr->renderer);
    r_display_frame(s_orche->r_mngr);
    f_timer_update(&f_timer);
    elapsed_time += f_timer.delta_time;
    if (elapsed_time >= 6000) {
      return SCENE_MAIN_MENU;
    }
  }

  return SCENE_MAIN_MENU;
}

// // [TODO] Should be in s_setup_results() but this being in r_manager and not
// s_game makes it tough. Fix once game has a scene
void s_setup_result_textures(S_Orchestrator* s_orche,
                             const ResultReport report) {
  SDL_Texture* winner_texture = NULL;
  SDL_Texture* loser_texture = NULL;

  winner_texture =
      s_orche->r_mngr->renderable_players[report.winner_idx].texture;
  loser_texture = s_orche->r_mngr->renderable_players[report.loser_idx].texture;

  s_orche->s_results->winner_txtr = winner_texture;
  s_orche->s_results->loser_txtr = loser_texture;
}

// [TODO] We want to have better return code handling and such - but that will
// be easier to do once game is migrated to being a scene. Until then, this will
// be kept ugly, since it can be, for the most part.
S_Scene_Code s_run_scene(S_Orchestrator* s_orche, S_Scene_Code s_scene_code) {
  if (s_scene_code == SCENE_MAIN_MENU) {
    int ret = s_run_main_menu_loop(s_orche);
    if (ret == RETURN_NEW_GAME) {
      return SCENE_GAME;
    } else if (ret == RETURN_QUIT) {
      return SCENE_QUIT;
    }
  } else if (s_scene_code == SCENE_GAME) {
    // [TODO] This should probably be moved to some "scene_setup()" function?
    F_Config cfg = {.r_vsync = 1, .r_max_fps = 200};
    setup_game_session(&s_orche->s_game->g_sess_mgr,
                       G_GAMETYPE_LOCAL_MULTIPLAYER, 120);
    register_game(s_orche->r_mngr, s_orche->s_game->game);
    int ret = run_game_session(&s_orche->s_game->g_sess_mgr, &cfg);
    r_unregister_game(s_orche->r_mngr);
    if (ret == 2) {
      return SCENE_RESULTS;
    }
  } else if (s_scene_code == SCENE_RESULTS) {
    ResultReport report = s_calc_result_report(s_orche);
    int succ =
        s_setup_results(s_orche->s_results, report, s_orche->r_mngr->renderer);
    s_setup_result_textures(
        s_orche, report);  // [TODO] DELME + better once game is scene
    if (!succ) {
      printf("[ERROR] [S_ORCHE] err initializing resultboard\n");
      return SCENE_QUIT;
    }
    S_Scene_Code ret = s_run_results_loop(s_orche);
    return ret;
  } else if (s_scene_code == SCENE_BOOT) {
    return s_run_boot_loop(s_orche);
  } else {
    printf("[ERROR] Attempted to run a scene with an unhandled scene code\n");
    return SCENE_QUIT;
  }

  printf("[ERROR] s_run_scene unhandled scene run return\n");
  return SCENE_QUIT;
}

// Highest-level function in the game engine, managing context of scenes running
// and switching between scenes based on events.
void s_orchestrate_scenes(S_Orchestrator* s_orche) {
  S_Scene_Code scene = SCENE_BOOT;
  int quit = 0;
  while (!quit) {
    scene = s_run_scene(s_orche, scene);
    if (scene == SCENE_QUIT) {
      quit = 1;
    }
  }
}

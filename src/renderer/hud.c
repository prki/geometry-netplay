#include "hud.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "font.h"
#include "text.h"

R_PlayerScore* r_new_r_playerscore(const Player* player, const R_Font* font,
                                   SDL_Renderer* renderer) {
  R_Text* text = NULL;
  if (player->player_ship.ship_color == BLUE) {
    text = new_r_text(renderer, "Score BLUE: 0", 32, font);
  } else if (player->player_ship.ship_color == RED) {
    text = new_r_text(renderer, "Score RED: 0", 32, font);
  }
  if (text == NULL) {
    printf("[ERROR] err new r_playerscore - text null\n");
    return NULL;
  }

  R_PlayerScore* r_ps = malloc(sizeof(R_PlayerScore));
  if (r_ps == NULL) {
    printf("[ERROR] malloc error new r_playerscore\n");
    r_destroy_text(text);
    return NULL;
  }

  r_ps->score_text = text;
  r_ps->pc_player = player;
  r_ps->score_font = font;
  r_ps->player_score = 0;

  return r_ps;
}

void r_destroy_r_playerscore(R_PlayerScore* r_ps) {
  if (r_ps != NULL) {
    r_destroy_text(r_ps->score_text);
    free(r_ps);
  }
}

R_HUD* new_r_hud(const FontStorage* font_storage) {
  R_HUD* hud = malloc(sizeof(R_HUD));
  if (hud == NULL) {
    printf("[ERROR] Error creating HUD - R_HUD malloc NULL\n");
    return NULL;
  }

  for (size_t i = 0; i < G_MAX_PC_PLAYERS; i++) {
    hud->player_scores[i] = NULL;
  }
  hud->hud_font = NULL;
  hud->font_storage = font_storage;
  hud->fps_text = NULL;
  hud->clock_text = NULL;

  return hud;
}

// Function initializing HUD with sensible values at the start of a game.
// Already creates renderable textures. As such, requires SDL Renderer to
// be passed.
// Function assumes that the parameter players[] is PC-players, hence its
// max size corresponds to the constant G_MAX_PC_PLAYERS. By doing so,
// indices of r_hud->player_scores are equivalent to players as their order
// is in the Game struct.
int r_initialize_hud(R_HUD* r_hud, SDL_Renderer* renderer,
                     const Player* players[], const F_Timer* f_timer) {
  if (r_hud == NULL) {
    printf("[ERROR] err initializing hud - r_hud == NULL\n");
    return 0;
  }

  R_Font* font = r_get_font_by_id(r_hud->font_storage, 0);
  if (font == NULL) {
    printf("[ERROR] Err init HUD - font ID 0 NULL\n");
    return 0;
  }

  r_hud->hud_font = font;
  r_hud->f_timer = f_timer;
  r_hud->fps_tick_timer = 30;
  r_hud->fps_ticks_to_render = 30;
  r_hud->fps_text = NULL;
  r_hud->elapsed_secs_memo = -1;
  r_hud->clock_text = NULL;

  for (size_t i = 0; i < G_MAX_PC_PLAYERS; i++) {
    if (players[i] != NULL) {
      R_PlayerScore* r_ps =
          r_new_r_playerscore(players[i], r_hud->hud_font, renderer);
      r_hud->player_scores[i] = r_ps;
    }
  }

  return 1;
}

// [TODO] Better handling of string length, validate that strncpy actually NULLs
// the entire char array
int r_playerscore_update(R_PlayerScore* r_ps, SDL_Renderer* renderer) {
  char score_str[256];
  strncpy(score_str, "\0", 256);
  if (r_ps->pc_player->player_ship.ship_color == BLUE) {
    sprintf(score_str, "Score BLUE: %d", r_ps->pc_player->score);
  } else if (r_ps->pc_player->player_ship.ship_color == RED) {
    sprintf(score_str, "Score RED: %d", r_ps->pc_player->score);
  }
  r_destroy_text(r_ps->score_text);

  R_Text* text = new_r_text(renderer, score_str, 32, r_ps->score_font);
  if (text == NULL) {
    printf("[ERROR] Error creating score text when updating HUD\n");
    return 0;
  }

  r_ps->score_text = text;
  return 1;
}

// [TODO] Validate error on playerscore update
int r_update_hud(R_HUD* r_hud, SDL_Renderer* renderer) {
  for (size_t i = 0; i < G_MAX_PC_PLAYERS; i++) {
    R_PlayerScore* curr_ps = r_hud->player_scores[i];
    if (curr_ps->player_score != curr_ps->pc_player->score) {
      r_playerscore_update(curr_ps, renderer);
      curr_ps->player_score = curr_ps->pc_player->score;
    }
  }

  r_hud->fps_tick_timer += 1;

  return 1;
}

// [TODO] On error, return failure
void r_render_hud_fps(R_HUD* r_hud, SDL_Renderer* renderer) {
  if (r_hud->fps_tick_timer >= r_hud->fps_ticks_to_render) {
    if (r_hud->fps_text != NULL) {
      r_destroy_text(r_hud->fps_text);
    }

    double fps = f_timer_calc_fps(r_hud->f_timer);
    char fps_str[128];
    strncpy(fps_str, "/0", 128);
    sprintf(fps_str, "FPS: %f", fps);
    R_Text* text = new_r_text(renderer, fps_str, 16, r_hud->hud_font);
    if (text == NULL) {
      printf("[ERROR] Error creating FPS text for HUD\n");
      return;
    }

    r_hud->fps_text = text;
    SDL_Rect dest_rect = {.x = 0,
                          .y = 20,
                          .w = r_hud->fps_text->texture_width,
                          .h = r_hud->fps_text->texture_height};
    SDL_RenderCopy(renderer, r_hud->fps_text->texture, NULL, &dest_rect);
    r_hud->fps_tick_timer = 0;
  } else {
    SDL_Rect dest_rect = {.x = 0,
                          .y = 20,
                          .w = r_hud->fps_text->texture_width,
                          .h = r_hud->fps_text->texture_height};
    SDL_RenderCopy(renderer, r_hud->fps_text->texture, NULL, &dest_rect);
  }
}

// [TODO] Check success/failure
// [TODO] Pass renderer manager instead of SDL renderer? That way, we could have
// metadata such as window screen?
void r_render_hud_time_elapsed(R_HUD* r_hud, SDL_Renderer* renderer,
                               unsigned int time_elapsed) {
  char timer_str[128];
  double time_elapsed_secs = floor(time_elapsed / 1000.0f);
  if ((long long int)time_elapsed_secs > r_hud->elapsed_secs_memo) {
    double time_elapsed_mins = floor(time_elapsed_secs / 60.0f);
    double time_elapsed_secs_remainder = fmod(time_elapsed_secs, 60.0);
    sprintf(timer_str, "Time: %02d:%02d", (int)time_elapsed_mins,
            (int)time_elapsed_secs_remainder);
    R_Text* text = new_r_text(renderer, timer_str, 24, r_hud->hud_font);
    if (text == NULL) {
      printf("[ERROR] Error creating clock text for HUD\n");
      return;
    }

    if (r_hud->clock_text != NULL) {
      r_destroy_text(r_hud->clock_text);
    }
    r_hud->clock_text = text;
    r_hud->elapsed_secs_memo = (long long int)time_elapsed_secs;
  }

  SDL_Rect dest_rect = {.x = 750,
                        .y = 0,
                        .w = r_hud->clock_text->texture_width,
                        .h = r_hud->clock_text->texture_height};
  SDL_RenderCopy(renderer, r_hud->clock_text->texture, NULL, &dest_rect);
}

// [TODO] On error, return failure
void r_render_hud(R_HUD* r_hud, SDL_Renderer* renderer,
                  unsigned int time_elapsed) {
  int score_gap = r_hud->player_scores[0]->score_text->texture_width +
                  64;  // x gap between each score
  for (size_t i = 0; i < G_MAX_PC_PLAYERS; i++) {
    const R_PlayerScore* curr_ps = r_hud->player_scores[i];
    if (curr_ps != NULL) {
      SDL_Rect dest_rect = {.x = 0 + i * score_gap,
                            .y = 0,
                            .w = curr_ps->score_text->texture_width,
                            .h = curr_ps->score_text->texture_height};
      SDL_RenderCopy(renderer, curr_ps->score_text->texture, NULL, &dest_rect);
    }
  }

  r_render_hud_fps(r_hud, renderer);
  r_render_hud_time_elapsed(r_hud, renderer, time_elapsed);
}

// Function freeing R_HUD* resources is safe to call on NULL
void destroy_r_hud(R_HUD* r_hud) {
  if (r_hud != NULL) {
    if (r_hud->fps_text != NULL) {
      r_destroy_text(r_hud->fps_text);
    }
    for (size_t i = 0; i < G_MAX_PC_PLAYERS; i++) {
      r_destroy_r_playerscore(r_hud->player_scores[i]);
    }
    if (r_hud->clock_text != NULL) {
      r_destroy_text(r_hud->clock_text);
    }

    free(r_hud);
  }
}

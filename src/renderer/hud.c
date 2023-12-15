#include "hud.h"

#include <stdio.h>
#include <string.h>

#include "font.h"
#include "text.h"

R_HUD* new_r_hud(const FontStorage* font_storage) {
  R_HUD* hud = malloc(sizeof(R_HUD));
  if (hud == NULL) {
    printf("[ERROR] Error creating HUD - R_HUD malloc NULL\n");
    return NULL;
  }

  hud->score_text = NULL;
  hud->hud_font = NULL;
  hud->font_storage = font_storage;

  return hud;
}

// Function initializing HUD with sensible values at the start of a game.
// Already creates renderable textures. As such, requires SDL Renderer to
// be passed.
int r_initialize_hud(R_HUD* r_hud, SDL_Renderer* renderer, const Player* player,
                     const F_Timer* f_timer) {
  if (r_hud == NULL) {
    printf("[ERROR] err initializing hud - r_hud == NULL\n");
    return 0;
  }

  R_Font* font = r_get_font_by_id(r_hud->font_storage, 0);
  if (font == NULL) {
    printf("[ERROR] Err init HUD - font ID 0 NULL\n");
    return 0;
  }

  R_Text* text = new_r_text(renderer, "Score 0", 8, font);
  if (text == NULL) {
    printf("[ERROR] Err init HUD - text NULL\n");
    return 0;
  }
  text->font = font;

  r_hud->score_text = text;
  r_hud->hud_font = font;
  r_hud->pc_player = player;
  r_hud->player_score = player->score;
  r_hud->f_timer = f_timer;
  r_hud->fps_tick_timer = 30;
  r_hud->fps_ticks_to_render = 30;
  r_hud->fps_text = NULL;

  return 1;
}

// [TODO] Better handling of string length
int r_update_hud_playerscore(R_HUD* r_hud, SDL_Renderer* renderer) {
  char score_str[256];
  strncpy(score_str, "\0", 256);
  sprintf(score_str, "Score %d", r_hud->pc_player->score);
  r_destroy_text(r_hud->score_text);

  R_Text* text = new_r_text(renderer, score_str, 10, r_hud->hud_font);
  if (text == NULL) {
    printf("[ERROR] Error creating text when updating HUD\n");
    return 0;
  }
  r_hud->score_text = text;

  return 1;
}

// [TODO] Better handling of string length
int r_update_hud(R_HUD* r_hud, SDL_Renderer* renderer) {
  if (r_hud->player_score != r_hud->pc_player->score) {
    r_update_hud_playerscore(r_hud, renderer);
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

// [TODO] On error, return failure
void r_render_hud(R_HUD* r_hud, SDL_Renderer* renderer) {
  SDL_Rect dest_rect = {.x = 0,
                        .y = 0,
                        .w = r_hud->score_text->texture_width,
                        .h = r_hud->score_text->texture_height};
  SDL_RenderCopy(renderer, r_hud->score_text->texture, NULL, &dest_rect);

  r_render_hud_fps(r_hud, renderer);
}

// Function freeing R_HUD* resources is safe to call on NULL
void destroy_r_hud(R_HUD* r_hud) {
  if (r_hud != NULL) {
    if (r_hud->score_text != NULL) {
      r_destroy_text(r_hud->score_text);
    }
    if (r_hud->fps_text != NULL) {
      r_destroy_text(r_hud->fps_text);
    }

    free(r_hud);
  }
}

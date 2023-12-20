#include "s_results.h"

#include <SDL.h>
#include <stdio.h>

#include "../renderer/font.h"
#include "../renderer/text.h"

S_Results* s_new_results(const FontStorage* r_fs, SDL_Renderer* renderer) {
  S_Results* s_results = malloc(sizeof(S_Results));
  if (s_results == NULL) {
    printf("[ERROR] malloc err s_new_results\n");
    return NULL;
  }
  s_results->header_text = NULL;
  s_results->winner_text = NULL;
  s_results->loser_text = NULL;
  s_results->menu_btn = NULL;
  s_results->rematch_btn = NULL;

  const R_Font* font = r_get_font_by_id(r_fs, 0);
  R_Text* text = new_r_text(renderer, "Pekelna parba game results", 64, font);
  if (text == NULL) {
    printf("[ERROR] err header text in s_new_results\n");
    free(s_results);
    return NULL;
  }

  UI_Button* btn_rematch = ui_new_button_from_paths(
      1200, 800, 300, 32, "./assets/menu/rematch.bmp",
      "./assets/menu/rematch_hover.bmp", NULL, renderer);
  if (btn_rematch == NULL) {
    printf("[ERROR] err s_new_results - new btn_rematch NULL\n");
    r_destroy_text(text);
    free(s_results);
    return NULL;
  }

  UI_Button* btn_back = ui_new_button_from_paths(
      100, 800, 300, 32, "./assets/menu/back_to_menu.bmp",
      "./assets/menu/back_to_menu_hover.bmp", NULL, renderer);
  if (btn_back == NULL) {
    printf("[ERROR] err s_new_results - new btn_back NULL\n");
    ui_destroy_button(btn_rematch);
    r_destroy_text(text);
    free(s_results);
    return NULL;
  }

  s_results->header_text = text;
  s_results->font = font;
  s_results->rematch_btn = btn_rematch;
  s_results->menu_btn = btn_back;

  return s_results;
}

void s_destroy_results(S_Results* s_results) {
  if (s_results != NULL) {
    if (s_results->header_text != NULL) {
      r_destroy_text(s_results->header_text);
    }
    if (s_results->rematch_btn != NULL) {
      ui_destroy_button(s_results->rematch_btn);
    }
    if (s_results->menu_btn != NULL) {
      ui_destroy_button(s_results->menu_btn);
    }
    free(s_results);
  }
}

// Function initializing the scene with a specific result report.
// Should be called before rendering results or running its loop.
// [TODO] Players should have names or some other proper differentiation.
int s_setup_results(S_Results* s_results, const ResultReport result_report,
                    SDL_Renderer* renderer) {
  char winner_buf[128];
  char loser_buf[128];
  sprintf(winner_buf, "Winner score: %d", result_report.score_winner);
  sprintf(loser_buf, "Loser score: %d", result_report.score_loser);
  R_Text* winner_text = new_r_text(renderer, winner_buf, 64, s_results->font);
  if (winner_text == NULL) {
    printf("[ERROR] [S_RESULTS] err creating winner text\n");
    return 0;
  }
  R_Text* loser_text = new_r_text(renderer, loser_buf, 64, s_results->font);
  if (loser_text == NULL) {
    printf("[ERROR] [S_RESULTS] err creating loser text\n");
    r_destroy_text(winner_text);
    return 0;
  }

  s_results->winner_text = winner_text;
  s_results->loser_text = loser_text;

  return 1;
}

void s_render_s_results(S_Results* s_results, SDL_Renderer* renderer) {
  int x = (1600 / 2) - (s_results->header_text->texture_width / 2);
  int y = 100;
  SDL_Rect dest_rect = {.x = 0, .y = y, .w = 32, .h = 32};

  r_render_text(s_results->header_text, renderer, x, y);

  y = 200;
  r_render_text(s_results->winner_text, renderer, x, y);
  dest_rect.x = x + s_results->winner_text->texture_width + 30;
  dest_rect.y = y - 8;
  SDL_RenderCopy(renderer, s_results->winner_txtr, NULL, &dest_rect);

  y = 300;
  dest_rect.y = y - 8;
  r_render_text(s_results->loser_text, renderer, x, y);
  SDL_RenderCopy(renderer, s_results->loser_txtr, NULL, &dest_rect);

  ui_render_button(s_results->menu_btn, renderer);
  ui_render_button(s_results->rematch_btn, renderer);
}

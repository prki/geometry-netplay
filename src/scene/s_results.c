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

  const R_Font* font = r_get_font_by_id(r_fs, 0);
  R_Text* text = new_r_text(renderer, "Pekelna parba game results", 64, font);
  if (text == NULL) {
    printf("[ERROR] err header text in s_new_results\n");
    free(s_results);
    return NULL;
  }
  s_results->header_text = text;

  return s_results;
}

void s_destroy_results(S_Results* s_results) {
  if (s_results != NULL) {
    if (s_results->header_text != NULL) {
      r_destroy_text(s_results->header_text);
    }
    free(s_results);
  }
}

void s_render_s_results(S_Results* s_results, SDL_Renderer* renderer) {
  int x = (1600 / 2) - (s_results->header_text->texture_width / 2);
  int y = 100;

  r_render_text(s_results->header_text, renderer, x, y);
}

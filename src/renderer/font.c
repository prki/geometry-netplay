#include "font.h"

#include <stdio.h>

#include "SDL_ttf.h"

R_Font* new_font(const char* file_path, int font_size) {
  if (TTF_Init() < 0) {
    printf("[ERROR] Error TTF Init: %s\n", TTF_GetError());
  }
  TTF_Font* ttf_font = TTF_OpenFont(file_path, font_size);
  if (ttf_font == NULL) {
    printf("[ERROR] Error new font from file path: %s: %s\n", file_path,
           TTF_GetError());
    return NULL;
  }

  R_Font* font = malloc(sizeof(R_Font));
  if (font == NULL) {
    printf("[ERROR] Error malloc font\n");
    TTF_CloseFont(ttf_font);
    return NULL;
  }

  font->font = ttf_font;

  return font;
}

void destroy_font(R_Font* font) {
  if (font != NULL) {
    if (font->font != NULL) {
      TTF_CloseFont(font->font);
    }
    TTF_Quit();
    free(font);
  }
}

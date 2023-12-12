#include "font.h"

#include <stdio.h>

#include "SDL_ttf.h"

// Initializes a new font. Note that the function expects TTF_Init() has
// already been called, as is managed by RendererManager (see
// new_renderer_manager())
R_Font* new_font(const char* file_path, int font_size) {
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
    free(font);
  }
}

FontStorage* r_new_font_storage(size_t cap) {
  if (cap == 0) {
    printf("[ERROR] cannot create new font storage with capacity 0\n");
    return NULL;
  }

  R_Font** fonts = malloc(cap * sizeof(R_Font*));
  if (fonts == NULL) {
    printf("[ERROR] malloc error font storage - font array\n");
    return NULL;
  }

  FontStorage* fs = malloc(sizeof(FontStorage));
  if (fs == NULL) {
    printf("[ERROR] malloc error font storage\n");
    free(fonts);
    return NULL;
  }

  fs->fonts = fonts;
  fs->fonts_cap = cap;
  fs->fonts_size = 0;

  return fs;
}

// Function also frees resources of each TTF_Font owned, if any are.
// Function is safe to call on NULL.
void r_destroy_font_storage(FontStorage* fs) {
  if (fs != NULL) {
    for (size_t i = 0; i < fs->fonts_size; i++) {
      destroy_font(fs->fonts[i]);
    }
    free(fs->fonts);
    free(fs);
  }
}

int r_insert_new_font(FontStorage* fs, R_Font* font) {
  if (fs == NULL) {
    printf("[ERROR] Attempted to insert font into NULL storage\n");
    return 0;
  }
  if (font == NULL) {
    printf("[ERROR] Attempted to insert NULL font into storage\n");
  }
  if (fs->fonts_size >= fs->fonts_cap) {
    printf("[ERROR] Cannot insert new font to storage, size %zu >= cap %zu\n",
           fs->fonts_size, fs->fonts_cap);
    return 0;
  }

  fs->fonts[fs->fonts_size] = font;
  fs->fonts_size += 1;

  return 1;
}

// Utility function for retrieving a specific font by ID.
// In the current simple implementation, ID is just index in the array.
// However this points to a possible hashmap-like implementation in the future.
R_Font* r_get_font_by_id(const FontStorage* fs, const size_t id) {
  if (fs == NULL) {
    printf("[ERROR] cant get font by id from NULL fs\n");
    return NULL;
  }
  if (id >= fs->fonts_size) {
    printf("[ERROR] cant get font by id %zu greater than fonts_size %zu\n", id,
           fs->fonts_size);
    return NULL;
  }

  return fs->fonts[id];
}

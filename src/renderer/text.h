#ifndef _TEXT_H_
#define _TEXT_H_
#include <SDL.h>

#include "font.h"

#define TEXT_CAPACITY 256

typedef struct {
  char* text_buffer;
  size_t text_len;
  size_t text_capacity;
  SDL_Color text_color;
  const R_Font* font;
  SDL_Texture* texture;
  int texture_width;
  int texture_height;
} R_Text;

R_Text* new_r_text(SDL_Renderer* renderer, const char* text_buffer,
                   size_t text_length, const R_Font* font);
char* change_text(R_Text* text, const char* text_buffer, size_t text_length);
SDL_Texture* create_text_texture(SDL_Renderer* renderer, R_Text* text,
                                 const SDL_Color color, int* out_wid,
                                 int* out_hei);
void r_render_text(const R_Text* text, SDL_Renderer* renderer, const int x,
                   const int y);
void r_destroy_text(R_Text* text);

#endif  // _TEXT_H_

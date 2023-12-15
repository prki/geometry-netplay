#include "text.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialize new r_text*. Note that under the hood, the function makes a call
// to strncpy. As such, up to text_length characters are copied from the
// text_buffer. The function fails and returns NULL if any of the pointers
// passed is NULL or if text_length is more than maximum allowed capacity,
// defined in the source file text.h.
R_Text* new_r_text(SDL_Renderer* renderer, const char* text_buffer,
                   size_t text_length, const R_Font* font) {
  if (font == NULL) {
    printf("[ERROR] Attempted new r_text with NULL r_font, return NULL\n");
    return NULL;
  }
  if (text_buffer == NULL) {
    printf("[ERROR] Attempted new r_text with NULL text buf, return NULL\n");
    return NULL;
  }
  if (text_length >= TEXT_CAPACITY) {
    printf("[ERROR] Attempted new r_text with len > max cap, return NULL\n");
    return NULL;
  }

  R_Text* ret = malloc(sizeof(R_Text));
  if (ret == NULL) {
    printf("[ERROR] malloc err r_text\n");
    return NULL;
  }

  ret->text_buffer = malloc(TEXT_CAPACITY * sizeof(char));
  if (ret->text_buffer == NULL) {
    printf("[ERROR] malloc err r_text->text_buffer\n");
    free(ret);
    return NULL;
  }
  strncpy(ret->text_buffer, text_buffer,
          text_length + 1);  // +1 to also copy 0 char
  ret->text_len = text_length;
  ret->font = font;

  SDL_Color white_color = {.r = 255, .g = 255, .b = 255, .a = 255};
  SDL_Texture* txtr = create_text_texture(
      renderer, ret, white_color, &ret->texture_width, &ret->texture_height);
  if (txtr == NULL) {
    printf("[ERROR] Err creating SDL Surface in new_r_text: %s\n",
           SDL_GetError());
    free(ret);
    return NULL;
  }
  ret->texture = txtr;

  return ret;
}

// Function creating an SDL_Texture out of R_Text. Function is unsafe
// to call with NULL parameters.
// Function provides output parameters out_wid and out_hei to provide
// dimensions of the text surface for rendering purposes.
SDL_Texture* create_text_texture(SDL_Renderer* renderer, R_Text* text,
                                 const SDL_Color color, int* out_wid,
                                 int* out_hei) {
  SDL_Surface* srfc = TTF_RenderUTF8_Solid(
      (TTF_Font*)text->font->font, (const char*)text->text_buffer, color);
  if (srfc == NULL) {
    printf("[ERROR] Err creating text srfc from text: %s\n", TTF_GetError());
    printf("[ERROR] Intended text was: %s\n", text->text_buffer);
    return NULL;
  }
  *out_wid = srfc->w;
  *out_hei = srfc->h;

  SDL_Texture* txtr = SDL_CreateTextureFromSurface(renderer, srfc);
  if (txtr == NULL) {
    printf("[ERROR] Err creating text texture from srfc: %s\n", TTF_GetError());
    SDL_FreeSurface(srfc);
    return NULL;
  }

  SDL_FreeSurface(srfc);

  return txtr;
}

// Utility function for changing text of an already-created R_Text. To be used
// to reduce the necessary amount of malloc calls.
// Function fails if text_length > r_text capacity. On fail, NULL is returned,
// but the R_Text* output parameter is not modified.
// This function is unsafe to call with NULL text or text_buffer.
// [TODO] Update texture
char* change_text(R_Text* text, const char* text_buffer, size_t text_length) {
  if (text_length > text->text_capacity) {
    printf("[ERROR] Changing text with one exceeding max capacity\n");
    return NULL;
  }

  strncpy(text->text_buffer, text_buffer,
          text_length + 1);  // +1 to also copy 0 char

  return text->text_buffer;
}

// [TODO] Text seems to destroy font, but should it actually own it?
void r_destroy_text(R_Text* text) {
  if (text != NULL) {
    if (text->text_buffer != NULL) {
      free(text->text_buffer);
    }
    SDL_DestroyTexture(text->texture);

    free(text);
  }
}

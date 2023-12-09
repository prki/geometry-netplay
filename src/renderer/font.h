#ifndef _FONT_H_
#define _FONT_H_
#include <SDL_ttf.h>

// Wrapper over SDL_ttf's TTF_Font. Provides functions for loading
// fonts and other capabilties. To be used by struct Text owning a specific
// font.
typedef struct {
  TTF_Font* font;
} R_Font;

R_Font* new_font(const char* file_path, int font_size);
void destroy_font(R_Font* font);

#endif  // _FONT_H_

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

// Font storage is implemented as a simple array. In case we were to use
// multiple fonts, it might be cool to have a hashmap with font IDs so we could
// request e.g. "Arial12Bold" and load it dynamically if need be. For initial
// architecture, it is completely fine to have just an array since it will be
// populated with one font. We may want to do more though.
typedef struct {
  R_Font** fonts;
  size_t fonts_size;
  size_t fonts_cap;
} FontStorage;

FontStorage* r_new_font_storage(size_t cap);
int r_insert_new_font(FontStorage* fs, R_Font* font);
R_Font* r_get_font_by_id(const FontStorage* fs, const size_t id);
void r_destroy_font_storage(FontStorage* fs);

#endif  // _FONT_H_

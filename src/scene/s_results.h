#ifndef _S_RESULTS_H_
#define _S_RESULTS_H_
#include <SDL.h>

#include "../renderer/font.h"
#include "../renderer/text.h"

typedef struct {
  R_Text* header_text;
} S_Results;

S_Results* s_new_results(const FontStorage* r_fs, SDL_Renderer* renderer);
void s_destroy_results(S_Results* s_results);
void s_render_s_results(S_Results* s_results, SDL_Renderer* renderer);

#endif  // _S_RESULTS_H_

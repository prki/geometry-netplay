#ifndef _HUD_H_
#define _HUD_H_
#include <SDL.h>

#include "text.h"

typedef struct {
  SDL_Texture* score_txtr;
  int score_txtr_wid;
  int score_txtr_hei;
  R_Text* score_text;
  R_Font* hud_font;
  int score;
} R_HUD;

R_HUD* new_r_hud(SDL_Renderer* renderer);
void destroy_r_hud(R_HUD* r_hud);

#endif  // _HUD_H_
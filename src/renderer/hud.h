#ifndef _HUD_H_
#define _HUD_H_
#include <SDL.h>

#include "../player.h"
#include "font.h"

typedef struct {
  SDL_Texture* score_txtr;
  int score_txtr_wid;
  int score_txtr_hei;
  const R_Font* hud_font;
  const FontStorage* font_storage;

  int player_score;  // [TODO] Become array once there are multiple players
  // Not owned
  // [TODO] Become array once there are multiple players
  const Player* pc_player;
} R_HUD;

// R_HUD* new_r_hud(SDL_Renderer* renderer, const Player* player);
R_HUD* new_r_hud(const FontStorage* font_storage);
int r_initialize_hud(R_HUD* r_hud, SDL_Renderer* renderer,
                     const Player* player);
int r_update_hud(R_HUD* r_hud, SDL_Renderer* renderer);
void r_render_hud(R_HUD* r_hud, SDL_Renderer* renderer);
void destroy_r_hud(R_HUD* r_hud);

#endif  // _HUD_H_

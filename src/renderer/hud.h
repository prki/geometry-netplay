#ifndef _HUD_H_
#define _HUD_H_
#include <SDL.h>

#include "../f_timer.h"
#include "../player.h"
#include "font.h"

// [TODO] I don't like three variables texture/wid/hei - those should be in one
// struct, possibly even within R_Text.
typedef struct {
  SDL_Texture* score_txtr;
  int score_txtr_wid;
  int score_txtr_hei;
  const R_Font* hud_font;
  const FontStorage* font_storage;
  const F_Timer* f_timer;
  int fps_tick_timer;
  int fps_ticks_to_render;
  SDL_Texture* fps_txtr;
  int fps_txtr_wid;
  int fps_txtr_hei;

  int player_score;  // [TODO] Become array once there are multiple players
  // Not owned
  // [TODO] Become array once there are multiple players
  const Player* pc_player;
} R_HUD;

// R_HUD* new_r_hud(SDL_Renderer* renderer, const Player* player);
R_HUD* new_r_hud(const FontStorage* font_storage);
int r_initialize_hud(R_HUD* r_hud, SDL_Renderer* renderer, const Player* player,
                     const F_Timer* f_timer);
int r_update_hud(R_HUD* r_hud, SDL_Renderer* renderer);
void r_render_hud(R_HUD* r_hud, SDL_Renderer* renderer);
void destroy_r_hud(R_HUD* r_hud);

#endif  // _HUD_H_

#ifndef _HUD_H_
#define _HUD_H_
#include <SDL.h>

#include "../f_timer.h"
#include "../game.h"
#include "../player.h"
#include "font.h"
#include "text.h"

// Wrapping struct over all items relevant to a player score for HUD rendering
// purposes.
// [TODO] Would be cool to also include player's ship texture/image in the HUD
// to make it easier to see who is P1/P2/...
typedef struct {
  R_Text* score_text;
  const R_Font* score_font;  // Not owned
  int player_score;  // Tracking score to check whether the actual Player* has a
                     // new value
  const Player* pc_player;  // Not owned
} R_PlayerScore;

R_PlayerScore* r_new_r_playerscore(const Player* player, const R_Font* font,
                                   SDL_Renderer* renderer);
void r_destroy_r_playerscore(R_PlayerScore* r_ps);

typedef struct {
  const R_Font* hud_font;
  const FontStorage* font_storage;
  const F_Timer* f_timer;
  int fps_tick_timer;
  int fps_ticks_to_render;
  R_Text* fps_text;
  R_PlayerScore* player_scores[G_MAX_PC_PLAYERS];
} R_HUD;

R_HUD* new_r_hud(const FontStorage* font_storage);
int r_initialize_hud(R_HUD* r_hud, SDL_Renderer* renderer,
                     const Player* players[], const F_Timer* f_timer);
int r_update_hud(R_HUD* r_hud, SDL_Renderer* renderer);
void r_render_hud(R_HUD* r_hud, SDL_Renderer* renderer,
                  unsigned int time_elapsed);
void destroy_r_hud(R_HUD* r_hud);

#endif  // _HUD_H_

#include "hud.h"

#include <stdio.h>

#include "font.h"
#include "text.h"

R_HUD* new_r_hud(const FontStorage* font_storage) {
  R_HUD* hud = malloc(sizeof(R_HUD));
  if (hud == NULL) {
    printf("[ERROR] Error creating HUD - R_HUD malloc NULL\n");
    return NULL;
  }

  hud->score_txtr = NULL;
  hud->score_txtr_wid = 0;
  hud->score_txtr_hei = 0;
  hud->hud_font = NULL;
  hud->score_text = NULL;
  hud->font_storage = font_storage;

  return hud;
}

// Function initializing HUD with sensible values at the start of a game.
// Already creates renderable textures. As such, requires SDL Renderer to
// be passed.
int r_initialize_hud(R_HUD* r_hud, SDL_Renderer* renderer) {
  if (r_hud == NULL) {
    printf("[ERROR] err initializing hud - r_hud == NULL\n");
    return 0;
  }

  R_Font* font = r_get_font_by_id(r_hud->font_storage, 0);
  if (font == NULL) {
    printf("[ERROR] Err init HUD - font ID 0 NULL\n");
    return 0;
  }

  R_Text* text = new_r_text("Score", 6, font);
  if (text == NULL) {
    printf("[ERROR] Err init HUD - text NULL\n");
    return 0;
  }
  text->font = font;

  SDL_Color color = {.r = 255, .g = 255, .b = 255, .a = 255};
  int wid, hei;
  SDL_Texture* txtr = create_text_texture(renderer, text, color, &wid, &hei);
  if (txtr == NULL) {
    printf("[ERROR] Err initializing HUD - r_hud text texture creation fail\n");
    return 0;
  }
  r_hud->score_txtr = txtr;
  r_hud->score_txtr_wid = wid;
  r_hud->score_txtr_hei = hei;
  r_hud->hud_font = font;
  r_hud->score_text = text;

  return 1;
}

void r_render_hud(R_HUD* r_hud, SDL_Renderer* renderer) {
  int wid = r_hud->score_txtr_wid;
  int hei = r_hud->score_txtr_hei;
  SDL_Rect dest_rect = {.x = 0, .y = 0, .w = wid, .h = hei};
  SDL_RenderCopy(renderer, r_hud->score_txtr, NULL, &dest_rect);
}

// Function freeing R_HUD* resources is safe to call on NULL
void destroy_r_hud(R_HUD* r_hud) {
  if (r_hud != NULL) {
    if (r_hud->score_txtr != NULL) {
      SDL_DestroyTexture(r_hud->score_txtr);
    }
    if (r_hud->score_text != NULL) {
      destroy_text(r_hud->score_text);
    }

    free(r_hud);
  }
}

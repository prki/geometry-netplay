#include "hud.h"

#include <stdio.h>

#include "font.h"
#include "text.h"

R_HUD* new_r_hud(SDL_Renderer* renderer) {
  R_Font* font = new_font("./assets/fonts/Arial.ttf", 12);
  if (font == NULL) {
    printf("[ERROR] Error creating HUD - font NULL\n");
    return NULL;
  }

  R_Text* text = new_r_text("Score", 6, font);
  if (text == NULL) {
    printf("[ERROR] Error creating HUD - text NULL\n");
    destroy_font(font);
    return NULL;
  }
  text->font = font;

  R_HUD* ret = malloc(sizeof(R_HUD));
  if (ret == NULL) {
    printf("[ERROR] Error creating HUD - R_HUD malloc NULL\n");
    destroy_text(text);
    return NULL;
  }

  SDL_Color color = {.r = 255, .g = 255, .b = 255, .a = 255};
  int wid, hei;
  SDL_Texture* txtr = create_text_texture(renderer, text, color, &wid, &hei);
  if (txtr == NULL) {
    printf("[ERROR] Error creating HUD - R_HUD creating text texture fail\n");
    destroy_text(text);
    free(ret);
    return NULL;
  }
  ret->score_txtr = txtr;
  ret->score_txtr_wid = wid;
  ret->score_txtr_hei = hei;
  ret->hud_font = font;
  ret->score_text = text;
  // ret->pc_player = player;

  return ret;
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

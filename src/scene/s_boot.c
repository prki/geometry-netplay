#include "s_boot.h"

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

S_Boot* s_new_boot(SDL_Renderer* renderer) {
  S_Boot* s_boot = malloc(sizeof(S_Boot));
  if (s_boot == NULL) {
    printf("[ERROR] Err s_new_boot - s_boot malloc NULL\n");
    return NULL;
  }
  s_boot->boot_image = NULL;

  s_boot->boot_image =
      r_new_image_from_path("./assets/boot.bmp", 1600, 900, renderer);
  if (s_boot->boot_image == NULL) {
    printf("[ERROR] Err s_new_boot - load boot image fail\n");
    s_destroy_boot(s_boot);
    return NULL;
  }

  return s_boot;
}

void s_destroy_boot(S_Boot* s_boot) {
  if (s_boot != NULL) {
    if (s_boot->boot_image != NULL) {
      r_destroy_image(s_boot->boot_image);
    }

    free(s_boot);
  }
}

void s_render_s_boot(S_Boot* s_boot, SDL_Renderer* renderer) {
  r_render_image(s_boot->boot_image, renderer, 0, 0);
}

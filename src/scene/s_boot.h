#ifndef _S_BOOT_H_
#define _S_BOOT_H_

#include "../renderer/r_image.h"

typedef struct {
  R_Image* boot_image;
} S_Boot;

S_Boot* s_new_boot(SDL_Renderer* renderer);
void s_destroy_boot(S_Boot* s_boot);
void s_render_s_boot(S_Boot* s_boot, SDL_Renderer* renderer);

#endif  // _S_BOOT_H_

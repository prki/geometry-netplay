#ifndef _R_IMAGE_H_
#define _R_IMAGE_H_

#include <SDL.h>

typedef struct {
  SDL_Texture* texture;
  size_t width;
  size_t height;
} R_Image;

R_Image* r_new_image(SDL_Texture* texture, size_t width, size_t height);
R_Image* r_new_image_from_path(const char* path, size_t width, size_t height,
                               SDL_Renderer* renderer);
void r_destroy_image(R_Image* r_image);
void r_render_image(const R_Image* r_image, SDL_Renderer* renderer, int x,
                    int y);

#endif  // _R_IMAGE_H_

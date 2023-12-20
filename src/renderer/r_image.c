#include "r_image.h"

#include <stdio.h>
#include <stdlib.h>

#include "../asset_loader.h"

R_Image* r_new_image(SDL_Texture* texture, size_t width, size_t height) {
  R_Image* r_image = malloc(sizeof(R_Image));
  if (r_image == NULL) {
    printf("[ERROR] err r_new_image - r_image malloc NULL\n");
    return NULL;
  }
  r_image->texture = texture;
  r_image->width = width;
  r_image->height = height;

  return r_image;
}

R_Image* r_new_image_from_path(const char* path, size_t width, size_t height,
                               SDL_Renderer* renderer) {
  R_Image* r_image = malloc(sizeof(R_Image));
  if (r_image == NULL) {
    printf("[ERROR] err r_new_image_from_path - r_image malloc NULL\n");
    return NULL;
  }
  r_image->width = width;
  r_image->height = height;

  r_image->texture = load_texture_from_bmp(renderer, path);
  if (r_image->texture == NULL) {
    printf("[ERROR] err r_new_image_from_path - loading texture failed\n");
    free(r_image);
    return NULL;
  }

  return r_image;
}

void r_destroy_image(R_Image* r_image) {
  if (r_image != NULL) {
    if (r_image->texture != NULL) {
      SDL_DestroyTexture(r_image->texture);
    }

    free(r_image);
  }
}

void r_render_image(const R_Image* r_image, SDL_Renderer* renderer, int x,
                    int y) {
  SDL_Rect dest_rect = {
      .x = x,
      .y = y,
      .w = r_image->width,
      .h = r_image->height,
  };
  SDL_RenderCopy(renderer, r_image->texture, NULL, &dest_rect);
}

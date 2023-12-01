#include "asset_loader.h"

#include <SDL.h>

SDL_Texture* load_texture_from_bmp(SDL_Renderer* renderer,
                                   const char* file_path) {
  if (renderer == NULL) {
    printf("[ERROR] Cannot load textures onto a NULL renderer\n");
    return NULL;
  }
  SDL_Surface* asset_surface = NULL;
  SDL_Texture* asset_texture = NULL;

  asset_surface = SDL_LoadBMP(file_path);
  if (asset_surface == NULL) {
    printf("[ERROR] Error loading texture from asset %s, SDL error: %s\n",
           file_path, SDL_GetError());
    return NULL;
  }

  Uint32 key_rgb = SDL_MapRGB(asset_surface->format, 0, 255, 0);
  int err = SDL_SetColorKey(asset_surface, SDL_TRUE, key_rgb);
  if (err) {
    printf("[ERROR] Error setting color key for asset %s, SDL error: %s\n",
           file_path, SDL_GetError());
    SDL_FreeSurface(asset_surface);
    return NULL;
  }

  asset_texture = SDL_CreateTextureFromSurface(renderer, asset_surface);
  if (asset_texture == NULL) {
    SDL_FreeSurface(asset_surface);
    printf("[ERROR] Error loading texture from asset %s, SDL error: %s\n",
           file_path, SDL_GetError());
    return NULL;
  }

  SDL_FreeSurface(asset_surface);
  return asset_texture;
}

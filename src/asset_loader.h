#ifndef _ASSET_LOADER_H
#define _ASSET_LOADER_H
#include <SDL.h>

SDL_Texture* load_texture_from_bmp(SDL_Renderer* renderer,
                                   const char* file_path);

#endif  // _ASSET_LOADER_H

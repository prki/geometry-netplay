#ifndef _GAME_WORLD_H_
#define _GAME_WORLD_H_
#include "math/collections.h"

typedef struct GameWorld {
  RectangleArray* rects;
} GameWorld;

GameWorld* new_game_world(void);
void destroy_game_world(GameWorld* world);

#endif  // _GAME_WORLD_H_

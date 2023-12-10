#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <SDL.h>

#include "bullets.h"
#include "math/vector.h"
#include "ship.h"

typedef struct Player {
  Ship player_ship;
  vec2d movement_direction;
  vec2d shoot_direction;
  int shoot_timer;
  int shoot_interval;
  int score;
} Player;

Player* new_player(vec2d ship_position, double hurtcirc_radius,
                   size_t ship_width, vec2d shoot_direction);
void destroy_player(Player* plr);
void update_player(Player* plr, BulletPool* bullet_pool);
void update_player_ai(Player* plr, BulletPool* bullet_pool);

#endif  // _PLAYER_H_

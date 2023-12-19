#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <SDL.h>

#include "bullets.h"
#include "f_controls.h"
#include "math/vector.h"
#include "ship.h"

typedef struct Player {
  Ship player_ship;
  vec2d movement_direction;
  vec2d shoot_direction;
  double shoot_timer;
  double shoot_interval;
  int score;
  F_Controls f_controls;
} Player;

Player* new_pc_player(vec2d ship_position, double hurtcirc_radius,
                      size_t ship_width, vec2d shoot_direction,
                      F_Controls controls, G_Ship_Color ship_color);
Player* new_ai_player(vec2d ship_position, double hurtcirc_radius,
                      size_t ship_width, vec2d shoot_direction);
void destroy_player(Player* plr);
void update_player(Player* plr, BulletPool* bullet_pool, double delta_time);
void update_player_ai(Player* plr, BulletPool* bullet_pool, double delta_time);

#endif  // _PLAYER_H_

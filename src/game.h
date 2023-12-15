#ifndef _GAME_H_
#define _GAME_H_
#include "bullets.h"
#include "game_event.h"
#include "game_world.h"
#include "player.h"

#define MAX_RECTS 300
#define MAX_AI_PLAYERS 16
#define SPAWN_POINTS_MAX_SIZE 32

typedef struct Game {
  Player* player;
  Player* enemy_players_ai[MAX_AI_PLAYERS];
  BulletPool* bullet_pool;
  GameEventQueue* evt_queue;
  GameWorld* game_world;
  vec2d spawn_points[SPAWN_POINTS_MAX_SIZE];
  size_t spawn_points_size;
} Game;

Game* initialize_game(void);
void destroy_game(Game* game);
void game_update(Game* game, double delta_time);
Player* add_enemy_player_ai(Game* game, const vec2d* ship_position,
                            double hurtcirc_radius, size_t ship_width,
                            vec2d shoot_direction);

#endif  // _GAME_H_

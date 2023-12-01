#ifndef _BULLETS_H_
#define _BULLETS_H_

#include "math/shapes.h"
struct Player;  // forward declaration due to bullet/player circular dependency

typedef struct Bullet {
  Rectangle hitbox;
  vec2d direction;
  double velocity;
  int is_active;
  struct Player* owner;
} Bullet;

void disable_bullet(Bullet* bullet);
void enable_bullet(Bullet* bullet);
void initialize_bullet(Bullet* bullet);
void update_bullet(Bullet* bullet);
void initialize_bullet_shot(Bullet* bullet, const vec2d direction,
                            const vec2d position);

typedef struct BulletPool {
  Bullet* bullets;
  unsigned int pool_size;
} BulletPool;

BulletPool* new_bullet_pool(void);
void destroy_bullet_pool(BulletPool* pool);
Bullet* get_inactive_bullet(BulletPool* pool);

#endif  // _BULLETS_H_

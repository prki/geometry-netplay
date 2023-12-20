#include "bullets.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "player.h"

// [TODO] To consider bullet velocity to be a member variable of e.g.
// gun/powerup/player
#define BULLET_VELOCITY 1200
#define BULLET_WIDTH 4
#define BULLET_HEIGHT 8

void disable_bullet(Bullet* bullet) {
  bullet->is_active = 0;
  bullet->hitbox = new_rectangle(0, 0, BULLET_WIDTH, BULLET_HEIGHT);
}
void enable_bullet(Bullet* bullet) { bullet->is_active = 1; }

// Initializes sensible base values for a bullet. Bullet initialized has NAN
// velocity/position/direction but a hitbox assigned. Bullet is not active after
// initialization.
// [TODO] Based on utilization of bullets in game logic, consider initialization
// to be moved to disable_bullet()
void initialize_bullet(Bullet* bullet) {
  bullet->hitbox = new_rectangle(0, 0, BULLET_WIDTH, BULLET_HEIGHT);
  bullet->direction = (vec2d){.x = NAN, .y = NAN};
  bullet->velocity = NAN;
  disable_bullet(bullet);
}

void update_bullet(Bullet* bullet, double delta_time) {
  if (bullet == NULL) {
    printf("[WARN] update_bullet() with NULL passed\n");
    return;
  }
  if (!bullet->is_active) {
    printf("[WARN] Attempted to update an inactive bullet\n");
    return;
  }

  vec2d direction = vec2d_normalize(&bullet->direction);
  vec2d trnsl_magnitude =
      (vec2d){.x = direction.x * bullet->velocity * delta_time,
              .y = direction.y * bullet->velocity * delta_time};

  translate_rectangle(&bullet->hitbox, trnsl_magnitude);
}

// Public function for "shooting" a bullet - setting the initial data.
// [TODO] Rotate rectangle so that bullet hitbox is rotated by angle of shot vs
// (0, -1)?
void initialize_bullet_shot(Bullet* bullet, const vec2d direction,
                            const vec2d position, Player* bullet_owner) {
  if (bullet == NULL) {
    printf("[WARN] initialize_shot_bullet() with NULL passed\n");
    return;
  }

  translate_rectangle(&bullet->hitbox, position);
  vec2d initial_direction = {.x = 0, .y = -1};  // "upwards"
  double rot_angle = vec2d_angle(&initial_direction, &direction);
  // rotation angle is [0; 180], but we need [0; 360] - if to the left of (0,
  // -1), recalculate
  if (direction.x < 0) {
    rot_angle = 180 + (180 - rot_angle);
  }
  rotate_rectangle(&bullet->hitbox, rot_angle);
  bullet->velocity = BULLET_VELOCITY;
  bullet->direction = direction;
  bullet->owner = bullet_owner;

  enable_bullet(bullet);
}

BulletPool* new_bullet_pool(void) {
  int pool_size = 1000;

  Bullet* bullets = malloc(pool_size * sizeof(Bullet));
  if (bullets == NULL) {
    printf("[ERROR] malloc fail new bullet pool bullets\n");
    return NULL;
  }

  for (int i = 0; i < pool_size; i++) {
    initialize_bullet(&bullets[i]);
  }

  BulletPool* pool = malloc(sizeof(BulletPool));
  if (pool == NULL) {
    printf("[ERROR] malloc fail BulletPool\n");
    free(bullets);
    return NULL;
  }

  pool->bullets = bullets;
  pool->pool_size = pool_size;

  return pool;
}

void destroy_bullet_pool(BulletPool* pool) {
  if (pool != NULL) {
    if (pool->bullets != NULL) {
      free(pool->bullets);
    }
    free(pool);
  }
}

// Public access function for obtaining a pointer to a pool which is disabled
// (i.e. available for utilization). If no bullet is inactive, NULL is returned.
Bullet* get_inactive_bullet(BulletPool* pool) {
  if (pool == NULL) {
    printf(
        "[WARN] Attempted to get inactive bullet from pool, but pool was "
        "NULL\n");
    return NULL;
  }

  for (unsigned int i = 0; i < pool->pool_size; i++) {
    if (pool->bullets[i].is_active == 0) {
      return &pool->bullets[i];
    }
  }

  return NULL;
}

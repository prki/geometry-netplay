#include "player.h"

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL_keyboard.h"
#include "math/shapes.h"
#include "math/vector.h"

#define PLAYER_VELOCITY 5

Player* new_player(vec2d ship_position, double hurtcirc_radius,
                   size_t ship_width, vec2d shoot_direction) {
  Player* plr = NULL;

  plr = malloc(sizeof(Player));
  if (plr == NULL) {
    printf("[ERROR] Error allocating player struct\n");
    return NULL;
  }

  plr->player_ship =
      create_ship(ship_position, ship_width, hurtcirc_radius, NULL, NULL, NULL);
  plr->shoot_direction = shoot_direction;
  plr->shoot_timer = 0;
  plr->shoot_interval = 6;
  plr->movement_direction = (vec2d){.x = 0, .y = 0};

  return plr;
}

void destroy_player(Player* plr) {
  if (plr != NULL) {
    free(plr);
  }
}

void update_shoot_timer(Player* plr) {
  if (plr->shoot_timer != 0) {
    plr->shoot_timer++;
  }
  if (plr->shoot_timer == plr->shoot_interval) {
    plr->shoot_timer = 0;
  }
}

// Function calculating the initial position of a bullet shot, i.e. where it
// should be set. As the bullet will end up being rotated based on player's
// shooting direction, it's position needs to be set based on the rectangle's
// middle point - where it's rotated around. As the rectangle initially is
// oriented north-wards, we need to discover it's topleft corner. We do so by
// finding the middle point to be, moving to the top edge and finally to the
// topleft corner.
// [TODO] Consider not calculating against collision circle, as those might
// (though unlikely) be different
vec2d calc_bullet_start(const Player* plr, const Bullet* bullet) {
  vec2d pos_mid = {
      .x = plr->player_ship.collision_circ.position.x +
           (plr->shoot_direction.x * (plr->player_ship.collision_circ.radius +
                                      bullet->hitbox.height / 2.0)),
      .y = plr->player_ship.collision_circ.position.y +
           (plr->shoot_direction.y * (plr->player_ship.collision_circ.radius +
                                      bullet->hitbox.height / 2.0))};

  vec2d dir_top = {.x = 0, .y = -1};
  vec2d pos_top = {.x = pos_mid.x + (dir_top.x * bullet->hitbox.height / 2.0),
                   .y = pos_mid.y + (dir_top.y * bullet->hitbox.height / 2.0)};
  vec2d dir_left = {.x = -1, .y = 0};
  vec2d pos_topleft = {
      .x = pos_top.x + (dir_left.x * bullet->hitbox.width / 2.0),
      .y = pos_top.y + (dir_left.y * bullet->hitbox.width / 2.0)};

  return pos_topleft;
}

void shoot_bullet(Player* plr, BulletPool* bullet_pool) {
  update_shoot_timer(plr);
  if (plr->shoot_timer != 0) {
    return;
  }

  Bullet* bullet = get_inactive_bullet(bullet_pool);
  if (bullet == NULL) {
    return;
  }
  bullet->is_active = 1;
  Bullet* bullet2 = get_inactive_bullet(bullet_pool);
  if (bullet2 == NULL) {
  }

  vec2d bullet_initial_position = calc_bullet_start(plr, bullet);
  vec2d bullet1_pos = rotate_point(bullet_initial_position,
                                   plr->player_ship.collision_circ.position, 0);
  vec2d bullet2_pos = rotate_point(bullet_initial_position,
                                   plr->player_ship.collision_circ.position, 0);
  vec2d bullet2_dir = vec2d_rotate(&plr->shoot_direction, 2);
  vec2d bullet1_dir = vec2d_rotate(&plr->shoot_direction, -2);
  bullet1_dir = vec2d_normalize(&bullet1_dir);
  bullet2_dir = vec2d_normalize(&bullet2_dir);  // Floating error or not?

  initialize_bullet_shot(bullet, bullet1_dir, bullet1_pos);
  initialize_bullet_shot(bullet2, bullet2_dir, bullet2_pos);

  plr->shoot_timer++;
}

void update_player(Player* plr, BulletPool* bullet_pool) {
  const Uint8* keystate = SDL_GetKeyboardState(NULL);
  vec2d movement = {.x = 0.0, .y = 0.0};
  vec2d new_direction = {.x = 0.0, .y = 0.0};

  if (keystate[SDL_SCANCODE_D]) {
    movement.x += 1.0;
  }
  if (keystate[SDL_SCANCODE_A]) {
    movement.x -= 1.0;
  }
  if (keystate[SDL_SCANCODE_W]) {
    movement.y -= 1.0;
  }
  if (keystate[SDL_SCANCODE_S]) {
    movement.y += 1.0;
  }
  if (keystate[SDL_SCANCODE_J]) {
    new_direction.x -= 1.0;
  }
  if (keystate[SDL_SCANCODE_K]) {
    new_direction.y += 1.0;
  }
  if (keystate[SDL_SCANCODE_L]) {
    new_direction.x += 1.0;
  }
  if (keystate[SDL_SCANCODE_I]) {
    new_direction.y -= 1.0;
  }

  if (movement.x != 0.0 && movement.y != 0.0) {
    movement = vec2d_normalize(&movement);
  }
  if (new_direction.x != 0.0 || new_direction.y != 0.0) {
    plr->shoot_direction = vec2d_normalize(&new_direction);
  }
  plr->movement_direction = movement;

  vec2d trnsl_magnitude = {.x = movement.x * PLAYER_VELOCITY,
                           .y = movement.y * PLAYER_VELOCITY};

  update_ship(&plr->player_ship, trnsl_magnitude);
  shoot_bullet(plr, bullet_pool);
}

void update_player_ai(Player* plr, BulletPool* bullet_pool) {
  shoot_bullet(plr, bullet_pool);
}

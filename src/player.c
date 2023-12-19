#include "player.h"

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL_keyboard.h"
#include "f_controls.h"
#include "math/shapes.h"
#include "math/vector.h"

#define PLAYER_VELOCITY 400

Player* new_pc_player(vec2d ship_position, double hurtcirc_radius,
                      size_t ship_width, vec2d shoot_direction,
                      F_Controls controls, G_Ship_Color ship_color) {
  Player* plr = NULL;

  plr = malloc(sizeof(Player));
  if (plr == NULL) {
    printf("[ERROR] Error allocating player struct\n");
    return NULL;
  }

  plr->player_ship = create_ship(ship_position, ship_width, hurtcirc_radius,
                                 NULL, NULL, NULL, ship_color);
  plr->shoot_direction = shoot_direction;
  plr->shoot_timer = 0;
  plr->shoot_interval = 70;
  plr->movement_direction = (vec2d){.x = 0, .y = 0};
  plr->score = 0;
  plr->f_controls = controls;

  return plr;
}

Player* new_ai_player(vec2d ship_position, double hurtcirc_radius,
                      size_t ship_width, vec2d shoot_direction) {
  Player* plr = NULL;

  plr = malloc(sizeof(Player));
  if (plr == NULL) {
    printf("[ERROR] malloc error new ai player\n");
    return NULL;
  }

  plr->player_ship = create_ship(ship_position, ship_width, hurtcirc_radius,
                                 NULL, NULL, NULL, BLUE);
  plr->shoot_direction = shoot_direction;
  plr->shoot_timer = 0;
  plr->shoot_interval = 70;
  plr->movement_direction = (vec2d){.x = 0, .y = 0};
  plr->score = 0;

  return plr;
}

void destroy_player(Player* plr) {
  if (plr != NULL) {
    free(plr);
  }
}

int update_shoot_timer(Player* plr, double delta_time) {
  int should_shoot = 0;
  if (plr->shoot_timer >= plr->shoot_interval) {
    should_shoot = 1;
  }
  if (should_shoot) {
    plr->shoot_timer = plr->shoot_timer - plr->shoot_interval;
  } else {
    plr->shoot_timer += delta_time * 1000;  // to ms
  }

  return should_shoot;
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

void shoot_bullet(Player* plr, BulletPool* bullet_pool, double delta_time) {
  int should_shoot = update_shoot_timer(plr, delta_time);
  if (!should_shoot) {
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

  initialize_bullet_shot(bullet, bullet1_dir, bullet1_pos, plr);
  initialize_bullet_shot(bullet2, bullet2_dir, bullet2_pos, plr);

  plr->shoot_timer++;
}

void update_player(Player* plr, BulletPool* bullet_pool, double delta_time) {
  const Uint8* keystate = SDL_GetKeyboardState(NULL);
  vec2d movement = {.x = 0.0, .y = 0.0};
  vec2d shoot_direction = {.x = 0.0, .y = 0.0};
  if (keystate[plr->f_controls.move_right]) {
    movement.x += 1.0;
  }
  if (keystate[plr->f_controls.move_left]) {
    movement.x -= 1.0;
  }
  if (keystate[plr->f_controls.move_down]) {
    movement.y += 1.0;
  }
  if (keystate[plr->f_controls.move_up]) {
    movement.y -= 1.0;
  }
  if (keystate[plr->f_controls.shoot_right]) {
    shoot_direction.x += 1.0;
  }
  if (keystate[plr->f_controls.shoot_left]) {
    shoot_direction.x -= 1.0;
  }
  if (keystate[plr->f_controls.shoot_down]) {
    shoot_direction.y += 1.0;
  }
  if (keystate[plr->f_controls.shoot_up]) {
    shoot_direction.y -= 1.0;
  }

  if (movement.x != 0.0 && movement.y != 0.0) {
    movement = vec2d_normalize(&movement);
  }
  if (shoot_direction.x != 0.0 || shoot_direction.y != 0.0) {
    plr->shoot_direction = vec2d_normalize(&shoot_direction);
  }
  plr->movement_direction = movement;

  vec2d trnsl_magnitude = {.x = movement.x * PLAYER_VELOCITY * delta_time,
                           .y = movement.y * PLAYER_VELOCITY * delta_time};

  update_ship(&plr->player_ship, trnsl_magnitude);
  shoot_bullet(plr, bullet_pool, delta_time);
}

void update_player_ai(Player* plr, BulletPool* bullet_pool, double delta_time) {
  shoot_bullet(plr, bullet_pool, delta_time);
}

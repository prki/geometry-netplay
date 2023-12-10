#include "game.h"

#include <stdio.h>

#include "game_event.h"
#include "math/collections.h"
#include "math/shapes.h"
#include "player.h"
#include "ship.h"

// [TODO] Figure out initialization/destruction pattern to prevent
// growth of continuous malloc/free tree
void _deinitialize_game(Game* game) {
  if (game->player != NULL) {
    destroy_player(game->player);
  }
  if (game->game_world != NULL) {
    destroy_game_world(game->game_world);
  }
  if (game->evt_queue != NULL) {
    destroy_game_event_queue(game->evt_queue);
  }
  if (game->bullet_pool != NULL) {
    destroy_bullet_pool(game->bullet_pool);
  }
  free(game);
}

Player* _initialize_pc_player(void) {
  vec2d initial_ship_position = {.x = 600, .y = 600};
  double hurtcirc_radius = 8;
  size_t ship_width = 32;
  vec2d shoot_direction = {.x = 0, .y = -1};

  return new_player(initial_ship_position, hurtcirc_radius, ship_width,
                    shoot_direction);
}

void _initialize_spawn_points(Game* game) {
  game->spawn_points[0] = (vec2d){.x = 800, .y = 800};
  game->spawn_points[1] = (vec2d){.x = 600, .y = 100};

  game->spawn_points_size = 2;
}

vec2d get_random_spawnpoint(Game* game) {
  int idx = rand() % game->spawn_points_size;

  return game->spawn_points[idx];
}

Game* initialize_game(void) {
  Game* game = malloc(sizeof(Game));
  if (game == NULL) {
    printf("[ERROR] Error malloc game struct\n");
    return NULL;
  }

  game->player = NULL;
  game->game_world = NULL;
  game->evt_queue = NULL;
  game->bullet_pool = NULL;

  _initialize_spawn_points(game);

  Player* player = _initialize_pc_player();
  if (player == NULL) {
    printf("[ERROR] Error initializing game while initializing player\n");
    _deinitialize_game(game);
    return NULL;
  }

  GameWorld* game_world = new_game_world();
  if (game_world == NULL) {
    printf("[ERROR] Error initializing game while initializing gameworld\n");
    _deinitialize_game(game);
    return NULL;
  }

  GameEventQueue* queue = new_game_event_queue();
  if (queue == NULL) {
    printf("[ERROR] Error initializing new evt queue for game\n");
    _deinitialize_game(game);
    return NULL;
  }

  BulletPool* bullet_pool = new_bullet_pool();

  game->player = player;
  game->game_world = game_world;
  game->evt_queue = queue;
  game->bullet_pool = bullet_pool;
  for (size_t i = 0; i < MAX_AI_PLAYERS; i++) {
    game->enemy_players_ai[i] = NULL;
  }

  return game;
}

Player* add_enemy_player_ai(Game* game, const vec2d* ship_position,
                            double hurtcirc_radius, size_t ship_width,
                            vec2d shoot_direction) {
  Player* tmp = NULL;
  vec2d spawn_pos;

  for (size_t i = 0; i < MAX_AI_PLAYERS; i++) {
    if (game->enemy_players_ai[i] == NULL) {
      tmp = game->enemy_players_ai[i];
      if (ship_position == NULL) {
        spawn_pos = game->spawn_points[0];
      } else {
        spawn_pos = *ship_position;
      }

      tmp = new_player(spawn_pos, hurtcirc_radius, ship_width, shoot_direction);
      if (tmp == NULL) {
        printf("[ERROR] new player failed when adding enemy player\n");
        return NULL;
      }

      game->enemy_players_ai[i] = tmp;

      return tmp;
    }
  }
  printf("[ERROR] Max AI players added, cant add new AI player\n");
  return NULL;
}

void destroy_game(Game* game) {
  if (game != NULL) {
    destroy_player(game->player);
    destroy_game_world(game->game_world);
    destroy_game_event_queue(game->evt_queue);
    destroy_bullet_pool(game->bullet_pool);
    for (size_t i = 0; i < MAX_AI_PLAYERS; i++) {
      if (game->enemy_players_ai[i] != NULL) {
        free(game->enemy_players_ai[i]);
      }
    }
    free(game);
  }
}

// [TODO] Split into smaller functions so that it's clear what the fuck this is
// doing
void correct_player_coords(Player* plr, const Rectangle* rect) {
  vec2d directions[2];  // rectangle axes
  rectangle_axes(directions, rect);
  vec2d player_point = {.x = plr->player_ship.collision_circ.position.x,
                        .y = plr->player_ship.collision_circ.position.y};
  for (int i = 0; i < 2; i++) {
    vec2d intersection[2];
    find_rect_line_intersection(intersection, rect, player_point,
                                directions[i]);
    if (!point_in_rectangle(&intersection[0],
                            rect)) {  // no intersection from this direction
      continue;
    }
    vec2d start_to_intersection = vec2d_from_points(
        intersection[0].x, intersection[0].y, player_point.x, player_point.y);
    vec2d correction_normal = vec2d_normal(&intersection[1]);
    correction_normal = vec2d_normalize(&correction_normal);
    if (vec2d_dot_product(&correction_normal, &start_to_intersection) < 0) {
      correction_normal = vec2d_scalar_multiply(&correction_normal, -1);
    }

    double dist_to_intersection = vec2d_magnitude(&start_to_intersection);
    double overlap =
        plr->player_ship.collision_circ.radius - dist_to_intersection;

    if (overlap < 1e-10) {
      break;
    }

    vec2d correction_vector =
        vec2d_scalar_multiply(&correction_normal, overlap);
    player_point.x += correction_vector.x;
    player_point.y += correction_vector.y;
  }

  const vec2d corners[4] = {rect->a1, rect->a2, rect->a3, rect->a4};
  for (int i = 0; i < 4; i++) {
    vec2d corner = corners[i];
    vec2d start_to_corner =
        vec2d_from_points(corner.x, corner.y, player_point.x, player_point.y);
    double len = vec2d_magnitude(&start_to_corner);
    if (len >= plr->player_ship.collision_circ.radius) {
      continue;  // no intersection with corner
    }
    double overlap = plr->player_ship.collision_circ.radius - len;
    start_to_corner = vec2d_normalize(&start_to_corner);
    vec2d correction_vector = vec2d_scalar_multiply(&start_to_corner, overlap);
    player_point.x += correction_vector.x;
    player_point.y += correction_vector.y;
  }

  move_ship_by_collision(&plr->player_ship, player_point);
}

void collide_player_rectangles(Game* game) {
  for (unsigned int i = 0; i < game->game_world->rects->size; i++) {
    Rectangle curr_rect = game->game_world->rects->array[i];
    if (circle_rectangle_intersection(&game->player->player_ship.collision_circ,
                                      &curr_rect)) {
      correct_player_coords(game->player, &curr_rect);
    }
  }
}

void update_players(Game* game) {
  // for player in players
  update_player(game->player, game->bullet_pool);
  // [TODO] Game should have a counter of AI players added so that this loop
  // is not iterating unnecessarily
  for (size_t i = 0; i < MAX_AI_PLAYERS; i++) {
    if (game->enemy_players_ai[i] != NULL) {
      update_player_ai(game->enemy_players_ai[i], game->bullet_pool);
    }
  }
  collide_player_rectangles(game);
}

// In case of collision, returns a pointer to rectangle a bullet is colliding
// with. In case of no collision, returns NULL
const Rectangle* collide_bullet_gameworld(GameWorld* world, Bullet* bullet) {
  for (unsigned int i = 0; i < world->rects->size; i++) {
    Rectangle* curr_rect = &world->rects->array[i];
    if (rectangle_intersection(curr_rect, &bullet->hitbox)) {
      return curr_rect;
    }
  }

  return NULL;
}

void _create_bullet_impact_event(Game* game, const Bullet* bullet,
                                 const Rectangle* colliding_rect) {
  vec2d intersection[2];
  vec2d bullet_center = rectangle_center(&bullet->hitbox);
  vec2d bullet_dir = bullet->direction;

  find_rect_line_intersection(intersection, colliding_rect, bullet_center,
                              bullet_dir);
  GameEvent* evt = new_bullet_impact_event(intersection[0]);
  if (evt == NULL) {
    printf("[WARN] Bullet impact event null => did not enqueue event\n");
    return;
  }

  evt_queue_enqueue(game->evt_queue, evt);
}

Player* collide_bullet_players(const Bullet* bullet, const Game* game) {
  // [TODO] Implement so that AI players are traversed only if active, not all
  for (size_t i = 0; i < MAX_AI_PLAYERS; i++) {
    if (game->enemy_players_ai[i] != NULL) {
      if (circle_rectangle_intersection(
              &game->enemy_players_ai[i]->player_ship.hurtcirc,
              &bullet->hitbox)) {
        return game->enemy_players_ai[i];
      }
    }
  }

  return NULL;
}

// [TODO] Player respawn should not lead to telefrag
// [TODO] Iterate score of bullet owner
void handle_player_death(Game* game, Player* player, Bullet* bullet) {
  disable_bullet(bullet);
  vec2d spawn_point = get_random_spawnpoint(game);
  GameEvent* evt =
      new_bullet_impact_event(player->player_ship.hurtcirc.position);
  if (evt == NULL) {
    printf("[WARN] player death bullet impact event null => no enqueue\n");
  } else {
    evt_queue_enqueue(game->evt_queue, evt);
  }

  relocate_ship(&player->player_ship, spawn_point);
}

void update_bullets(Game* game) {
  for (size_t i = 0; i < game->bullet_pool->pool_size; i++) {
    Bullet* curr_bullet = &game->bullet_pool->bullets[i];
    if (curr_bullet->is_active) {
      update_bullet(curr_bullet);
      const Rectangle* colliding_rect =
          collide_bullet_gameworld(game->game_world, curr_bullet);
      if (colliding_rect != NULL) {
        _create_bullet_impact_event(game, curr_bullet, colliding_rect);
        disable_bullet(curr_bullet);
      }

      Player* player_hit = collide_bullet_players(curr_bullet, game);
      if (player_hit != NULL) {
        handle_player_death(game, player_hit, curr_bullet);
        Player* bullet_owner = curr_bullet->owner;
        bullet_owner->score += 1;
        printf("Updated bullet owner score to: %d\n", bullet_owner->score);
      }
    }
  }
}

void game_update(Game* game) {
  update_players(game);
  update_bullets(game);
}
